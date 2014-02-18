//coding: utf-8
#include "LinesList.h"
#include <iostream>
#include <string>
#include <sstream>

CLinesList::CLinesList(IRenderingObjectComm *obj, CScreenTransport* commonScreen) : 
    IRenderingObject(obj)
{
    this->CommonScreen = commonScreen;

    this->LinesTitle = NULL;
    this->LinesDirection = NULL;
    
    this->ScrollBar = new CScrollBar(obj, this);
}

CLinesList::~CLinesList()
{
    delete this->ScrollBar;
}

void CLinesList::Init()
{
    // Position du menu sur l'écran
    float left = 0.0093;
    float top = 0.13;
    float right = 0.26; // 0.297 avec la scrollbar
    float bottom = 0.98;
    
    this->Width = (right - left) * this->CommonScreen->GetScreenWidth();
    this->Height = (bottom - top) * this->CommonScreen->GetScreenHeight();
    
    float separatorHeight = 0.002 * this->CommonScreen->GetScreenHeight();
    
    // Calcul des coefficients du scissor test
    int moduleHeight = this->CommonScreen->GetScreenPixelHeight() - this->CommonScreen->GetTaskbarHeight();
    
    this->LeftCrop = left * this->CommonScreen->GetScreenPixelWidth();
    this->BottomCrop = (1 - bottom) * moduleHeight + this->CommonScreen->GetTaskbarHeight(); // Parce que le repère est en bas à gauche
    this->RightCrop = (right - left) * this->CommonScreen->GetScreenPixelWidth();
    this->TopCrop = (bottom - top) * moduleHeight;
    
    // Calibrage du ModelView
    this->MVPmatrix = OpenUtility::CMat4x4<float>(this->CommonScreen->GetMVPMatrix());
    this->MVPmatrix *= OpenUtility::CMat4x4<float>().SetTranslate(left * this->CommonScreen->GetScreenWidth(), top * this->CommonScreen->GetScreenHeight(), 0);
        
    this->Separator = new OpenUtility::CQuad(0, 0, this->Width, separatorHeight, CScreenTransport::DARKGREEN);
    
    // Textures
    OpenUtility::CVector<OpenUtility::CTextureMultiQuad::SQuad> logos;

    std::vector<CLine>* lines = this->CommonScreen->GetLines();
    OpenUtility::CTexture* tex = this->CommonScreen->GetTexture((*lines)[0].tileset);
    int nb = lines->size();
    
    for (int i=0; i<nb; i++)
        logos.Add(new OpenUtility::CTextureMultiQuad::SQuad((*lines)[i].left, tex->GetH() - (*lines)[i].top - (*lines)[i].height + 1, (*lines)[i].width - 2, (*lines)[i].height - 2, 1.5));
        
    this->LineLogo = new OpenUtility::CTextureMultiQuad(tex, logos);
    
    // Textes    
    this->LinesTitle = new OpenUtility::C3DText(this->CommonScreen->GetFont(FT_MENUTITLE), 0.4);    
    this->LinesTitle->SetDefaultShaderColor(CScreenTransport::DARKGREEN);   
    this->LinesTitle->SetAlignement(OpenUtility::CFontLoader::IFontEngine::EHAlignLeft, OpenUtility::CFontLoader::IFontEngine::EVAlignTop);
    
    this->LinesDirection = new OpenUtility::C3DText(this->CommonScreen->GetFont(FT_MENU), 0.35);    
    this->LinesDirection->SetDefaultShaderColor(CScreenTransport::DARKGREEN);   
    this->LinesDirection->SetAlignement(OpenUtility::CFontLoader::IFontEngine::EHAlignLeft, OpenUtility::CFontLoader::IFontEngine::EVAlignMiddle);
    
    this->CurrentScreen = this->CommonScreen->GetStatus()->CurrentScreen;
    
    this->Update(true);
    
    this->ScrollBar->Init();
}

void CLinesList::UnInit()
{
    delete this->Separator;
    delete this->LineLogo;
    
    delete this->LinesTitle;
    delete this->LinesDirection;
    
    this->ScrollBar->UnInit();
}

bool CLinesList::PreRender()
{
    bool mustRender = false;
    
    mustRender = mustRender || this->ScrollBar->PreRender();
    
    // Gestion du changement d'écran
    if (this->CommonScreen->GetStatus()->CurrentScreen != this->CurrentScreen) {
        mustRender = true;
        this->Update(true);
        this->CurrentScreen = this->CommonScreen->GetStatus()->CurrentScreen;
    }
    
    if (mustRender)
        this->Update(false);
    
    return mustRender;
}

void CLinesList::Update(bool screenChanged)
{      
    // Gestion de la hauteur de contenu
    if (screenChanged) {
        if (this->CommonScreen->GetStatus()->CurrentScreen == ST_HOME) {
            this->ContentHeight = (0.09 + this->CommonScreen->GetLines()->size() * 0.11 - 0.11 / 2) * this->CommonScreen->GetScreenHeight(); // Liste
            //this->ContentHeight += 0.04; // Espacement entre listes
        }
        
        this->ScrollBar->ScrollTo(0.5);
    }  
    
    // MAJ du scroll
    this->ScrollMatrix = OpenUtility::CMat4x4<float>(this->MVPmatrix);
    this->ScrollMatrix *= OpenUtility::CMat4x4<float>().SetTranslate(0, (this->Height - this->ContentHeight) * this->ScrollBar->GetCurrentScroll(), 0);
}

void CLinesList::Render()
{
    this->ScrollBar->Render();
    this->CurrentHeight = (this->Height - this->ContentHeight) * this->ScrollBar->GetCurrentScroll();
           
    glScissor(this->LeftCrop, this->BottomCrop, this->RightCrop, this->TopCrop);
    glEnable(GL_SCISSOR_TEST);
    
    this->ListMatrix = OpenUtility::CMat4x4<float>(this->ScrollMatrix);
    
    if (this->CommonScreen->GetStatus()->CurrentScreen == ST_HOME) {
        // Draw all lines   
        this->RenderTitle("Lignes");
        
        std::vector<CLine>* lines = this->CommonScreen->GetLines();
        int nb = lines->size();
        for (int i=0; i<nb; i++) {    
            if (this->CurrentHeight > this->Height + 0.11 * this->CommonScreen->GetScreenHeight() && this->CurrentHeight <= 0)
                this->RenderLine(i, (*lines)[i].direction1, (*lines)[i].direction2);
            this->ListMatrix *= OpenUtility::CMat4x4<float>().SetTranslate(0, 0.11 * this->CommonScreen->GetScreenHeight(), 0);
            this->CurrentHeight += 0.11 * this->CommonScreen->GetScreenHeight();
        }
    } else {
        if (this->CommonScreen->GetStatus()->CurrentLine != "") {
            if (this->CommonScreen->GetStatus()->CurrentStop != "") {
                // Draw StopConnected
                std::string title = "Correspondances à "; title += this->CommonScreen->GetStatus()->CurrentStop;
                this->RenderTitle(title);
                
                
            } else {
                // Draw LineConnected         
                this->RenderTitle("Correspondances");    
                
                
            }
        }
        
        // Draw near lines
        this->ListMatrix *= OpenUtility::CMat4x4<float>().SetTranslate(-0.026 * this->CommonScreen->GetScreenWidth(), -0.02 * this->CommonScreen->GetScreenHeight(), 0);
        this->RenderTitle("Lignes à proximité");
        
        
        
        // Draw other lines
        this->ListMatrix *= OpenUtility::CMat4x4<float>().SetTranslate(-0.026 * this->CommonScreen->GetScreenWidth(), -0.02 * this->CommonScreen->GetScreenHeight(), 0);
        this->RenderTitle("Autres lignes");
        
        
    }
    
    glDisable(GL_SCISSOR_TEST);
}

void CLinesList::RenderTitle(std::string text)
{
    this->LinesTitle->SetDefaultShaderMatrix(this->ListMatrix);
    this->LinesTitle->UpdateText(text.c_str());
    this->LinesTitle->Draw();

    // Séparateur
    this->ListMatrix *= OpenUtility::CMat4x4<float>().SetTranslate(0, 0.03 * this->CommonScreen->GetScreenHeight(), 0);
    this->Separator->SetDefaultShaderMatrix(this->ListMatrix, OpenUtility::CMat4x4<float>());
    this->Separator->Draw();

    this->ListMatrix *= OpenUtility::CMat4x4<float>().SetTranslate(0.026 * this->CommonScreen->GetScreenWidth(), 0.06 * this->CommonScreen->GetScreenHeight(), 0);
    
    this->CurrentHeight += 0.09 * this->CommonScreen->GetScreenHeight();
}

void CLinesList::RenderLine(int texId, std::string direction1, std::string direction2)
{
    pClass->GetGlobalShader().UseProgram();
    glUniformMatrix4fv(pClass->GetGlobalShader()["u_MVPmatrix"], 1, GL_FALSE, this->ListMatrix.GetMatrix());    
    LineLogo->AttachAttribToData(pClass->GetGlobalShader()["vPos"], pClass->GetGlobalShader()["vNorm"], pClass->GetGlobalShader()["vTexCoord"]);
    this->LineLogo->Draw(texId);

    // Directions
    this->ListMatrix *= OpenUtility::CMat4x4<float>().SetTranslate(0.03 * this->CommonScreen->GetScreenWidth(), -0.019 * this->CommonScreen->GetScreenHeight(), 0);
    this->LinesDirection->SetDefaultShaderMatrix(this->ListMatrix);
    this->LinesDirection->UpdateText(direction1.c_str());
    this->LinesDirection->Draw();

    this->ListMatrix *= OpenUtility::CMat4x4<float>().SetTranslate(0, 0.038 * this->CommonScreen->GetScreenHeight(), 0);
    this->LinesDirection->SetDefaultShaderMatrix(this->ListMatrix);
    this->LinesDirection->UpdateText(direction2.c_str());
    this->LinesDirection->Draw();

    this->ListMatrix *= OpenUtility::CMat4x4<float>().SetTranslate(-0.03 * this->CommonScreen->GetScreenWidth(), -0.019 * this->CommonScreen->GetScreenHeight(), 0);
}