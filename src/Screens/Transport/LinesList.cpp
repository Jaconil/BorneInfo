//coding: utf-8
#include "LinesList.h"
#include <iostream>
#include <sstream>

CLinesList::CLinesList(IRenderingObjectComm *obj, CScreenTransport* commonScreen) : 
    IRenderingObject(obj)
{
    this->CommonScreen = commonScreen;
    
    this->StopConnectedTitle = NULL;
    this->LineConnectedTitle = NULL;
    this->NearLinesTitle = NULL;
    this->OtherLinesTitle = NULL;
    this->AllLinesTitle = NULL;
    
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
    float bottom = 0.8;
    
    this->Width = (right - left) * this->CommonScreen->GetScreenWidth();
    this->Height = (bottom - top) * this->CommonScreen->GetScreenHeight();
    
    // Calcul des coefficients du scissor test
    int moduleHeight = this->CommonScreen->GetScreenPixelHeight() - this->CommonScreen->GetTaskbarHeight();
    
    this->LeftCrop = left * this->CommonScreen->GetScreenPixelWidth();
    this->BottomCrop = (1 - bottom) * moduleHeight + this->CommonScreen->GetTaskbarHeight(); // Parce que le repère est en bas à gauche
    this->RightCrop = (right - left) * this->CommonScreen->GetScreenPixelWidth();
    this->TopCrop = (bottom - top) * moduleHeight;
    
    // Calibrage du ModelView
    this->MVPmatrix = OpenUtility::CMat4x4<float>(this->CommonScreen->GetMVPMatrix());
    this->MVPmatrix *= OpenUtility::CMat4x4<float>().SetTranslate(left * this->CommonScreen->GetScreenWidth(), top * this->CommonScreen->GetScreenHeight(), 0);
    
    this->Background = new OpenUtility::CQuad(0, 0, this->Width, this->Height, CScreenTransport::DARKGREEN);
    this->Background->SetDefaultShaderMatrix(this->MVPmatrix, OpenUtility::CMat4x4<float>());
    
    this->StopConnectedTitle = new OpenUtility::C3DText(this->CommonScreen->GetFont(FT_MENUTITLE), 0.4);
    this->LineConnectedTitle = new OpenUtility::C3DText(this->CommonScreen->GetFont(FT_MENUTITLE), 0.4);
    this->NearLinesTitle = new OpenUtility::C3DText(this->CommonScreen->GetFont(FT_MENUTITLE), 0.4);
    this->OtherLinesTitle = new OpenUtility::C3DText(this->CommonScreen->GetFont(FT_MENUTITLE), 0.4);
    this->AllLinesTitle = new OpenUtility::C3DText(this->CommonScreen->GetFont(FT_MENUTITLE), 0.4);
    
    this->StopConnectedTitle->SetDefaultShaderColor(CScreenTransport::BLACK);
    this->LineConnectedTitle->SetDefaultShaderColor(CScreenTransport::BLACK);
    this->NearLinesTitle->SetDefaultShaderColor(CScreenTransport::BLACK);
    this->OtherLinesTitle->SetDefaultShaderColor(CScreenTransport::BLACK);
    this->AllLinesTitle->SetDefaultShaderColor(CScreenTransport::BLACK);
        
    this->StopConnectedTitle->SetAlignement(OpenUtility::CFontLoader::IFontEngine::EHAlignLeft, OpenUtility::CFontLoader::IFontEngine::EVAlignTop);
    this->LineConnectedTitle->SetAlignement(OpenUtility::CFontLoader::IFontEngine::EHAlignLeft, OpenUtility::CFontLoader::IFontEngine::EVAlignTop);
    this->NearLinesTitle->SetAlignement(OpenUtility::CFontLoader::IFontEngine::EHAlignLeft, OpenUtility::CFontLoader::IFontEngine::EVAlignTop);
    this->OtherLinesTitle->SetAlignement(OpenUtility::CFontLoader::IFontEngine::EHAlignLeft, OpenUtility::CFontLoader::IFontEngine::EVAlignTop);
    this->AllLinesTitle->SetAlignement(OpenUtility::CFontLoader::IFontEngine::EHAlignLeft, OpenUtility::CFontLoader::IFontEngine::EVAlignTop);
    
    this->StopConnectedTitle->UpdateText("Correspondances à X");
    this->LineConnectedTitle->UpdateText("Correspondances");
    this->NearLinesTitle->UpdateText("Lignes à proximité");
    this->OtherLinesTitle->UpdateText("Autres lignes");
    this->AllLinesTitle->UpdateText("Lignes");
    
    this->CurrentScreen = this->CommonScreen->GetStatus()->CurrentScreen;
    
    this->Update();
    
    this->ScrollBar->Init();
}

void CLinesList::UnInit()
{
    delete this->Background;
    
    delete this->StopConnectedTitle;
    delete this->LineConnectedTitle;
    delete this->NearLinesTitle;
    delete this->OtherLinesTitle;
    delete this->AllLinesTitle;
    
    this->ScrollBar->UnInit();
}

bool CLinesList::PreRender()
{
    bool mustRender = false;
    
    this->Update();
    
    mustRender = mustRender || this->ScrollBar->PreRender();
    
    // Gestion du changement d'écran
    if (this->CommonScreen->GetStatus()->CurrentScreen != this->CurrentScreen) {
        mustRender = true;
        this->CurrentScreen = this->CommonScreen->GetStatus()->CurrentScreen;
        this->ScrollBar->ScrollTo(0.5);
        // MAJ du scroll
        this->MVPmatrix *= OpenUtility::CMat4x4<float>().SetTranslate(0, (this->Height - this->ContentHeight) * this->ScrollBar->GetCurrentScroll(), 0);
    }
    
    return mustRender;
}

void CLinesList::Update()
{      
    if (this->CommonScreen->GetStatus()->CurrentScreen == ST_HOME) {
        this->ContentHeight = 19 * this->Height / 15.7;
    } else {
        this->ContentHeight = 29 * this->Height / 15.7;
    }    
}

void CLinesList::Render()
{
    this->Background->Draw();
    this->ScrollBar->Render();
       
    glScissor(this->LeftCrop, this->BottomCrop, this->RightCrop, this->TopCrop);
    glEnable(GL_SCISSOR_TEST);
    
    if (this->CommonScreen->GetStatus()->CurrentScreen == ST_HOME) {
        // Draw all lines
        
//        this->AllLinesTitle->SetDefaultShaderMatrix(this->MVPmatrix);
//        this->AllLinesTitle->Draw();
        for (int i=0; i<20; i++) {
            ListMatrix = OpenUtility::CMat4x4<float>(this->MVPmatrix);
            ListMatrix *= OpenUtility::CMat4x4<float>().SetTranslate(0, i * this->Height / 15.7, 0);
            this->AllLinesTitle->SetDefaultShaderMatrix(this->ListMatrix);
            
            std::stringstream t;
            t << "Lignes ";
            t << (20-i);
            
            this->AllLinesTitle->UpdateText(t.str().c_str());
            this->AllLinesTitle->Draw();
        }
        
    } else {
        if (this->CommonScreen->GetStatus()->CurrentLine != "") {
            if (this->CommonScreen->GetStatus()->CurrentStop != "") {
                // Draw StopConnected
            } else {
                // Draw LineConnected
                
                for (int i=0; i<30; i++) {
                    ListMatrix = OpenUtility::CMat4x4<float>(this->MVPmatrix);
                    ListMatrix *= OpenUtility::CMat4x4<float>().SetTranslate(0, i * this->Height / 15.7, 0);
                    this->AllLinesTitle->SetDefaultShaderMatrix(this->ListMatrix);

                    std::stringstream t;
                    t << "Correspondances ";
                    t << (30-i);

                    this->AllLinesTitle->UpdateText(t.str().c_str());
                    this->AllLinesTitle->Draw();
                }
            }
        }
        
        // Draw near lines
        // Draw other lines
    }
    
    glDisable(GL_SCISSOR_TEST);
}
