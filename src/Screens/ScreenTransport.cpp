//coding: utf-8
#include "ScreenTransport.h"
#include "Transport/ScreenHome.h"
#include "Transport/ScreenSchedule.h"
#include <iostream>


const OpenUtility::RGBAd CScreenTransport::WHITE(1.0, 1.0, 1.0, 1.0);
const OpenUtility::RGBAd CScreenTransport::BLACK(0.0, 0.0, 0.0, 1.0);
const OpenUtility::RGBAd CScreenTransport::DARKGREEN(0.0, 0.26, 0.32, 1.0);
const OpenUtility::RGBAd CScreenTransport::GREY(0.73, 0.73, 0.73, 1.0);


CScreenTransport::CScreenTransport(IRenderingObjectComm *obj,OpenUtility::ITextureQuad *image) :
	IScreen(obj, "Transports", image)
{
    this->Background = NULL;
    this->Header = NULL;
    this->Logo = NULL;
    this->Title = NULL;
        
    this->Fonts[FT_TITLE] = NULL;
    this->Fonts[FT_MENUTITLE] = NULL;
    this->Fonts[FT_MENU] = NULL;
    
    this->Screens[ST_HOME] = new CTScreenHome(obj, this);
    this->Screens[ST_SCHEDULE] = new CTScreenSchedule(obj, this);
    this->Screens[ST_STOPS] = new CTScreenHome(obj, this);
    this->Screens[ST_TRAFFIC] = new CTScreenHome(obj, this);
    this->Screens[ST_LINEMAP] = new CTScreenHome(obj, this);
    this->Screens[ST_MAP] = new CTScreenHome(obj, this);
    
    this->LinesList = new CLinesList(obj, this);
}

CScreenTransport::CScreenTransport(const CScreenTransport &obj) :
	IScreen(obj)   
{
}

CScreenTransport::~CScreenTransport()
{
    delete[] this->Screens;
    delete this->LinesList;
}

CScreenTransport& CScreenTransport::operator=(const CScreenTransport &obj)
{
	IScreen::operator=(obj);
	return(*this);
}

void CScreenTransport::Init()
{
    // Calibrage des coordonnées
    // Coin haut-gauche : 0, 0
    // Coin bas-droite  : ScreenWidth, ScreenHeight
    const OpenUtility::SFrustum &Frustum = pClass->GetFrustum();
    double ratio = Frustum.pFar / Frustum.pNear;

    ModelViewMatrix *= OpenUtility::CMat4x4<float>().SetTranslate(Frustum.pLeft * ratio / 2.0, Frustum.pTop * ratio / 2.0, 0);
    this->MVPMatrix = OpenUtility::CMat4x4<float>(ProjectionMatrix);
    this->MVPMatrix *= ModelViewMatrix;

    this->ScreenWidth = Frustum.pRight * ratio;
    this->ScreenHeight = Frustum.pBottom * ratio * 0.9; // Coefficient pour retirer la hauteur de la taskbar

    // Chargement des polices
    this->Fonts[FT_TITLE] = new OpenUtility::CFontLoader("../content/Transport/trebucbd.ttf", 80);
    this->Fonts[FT_MENUTITLE] = new OpenUtility::CFontLoader("../content/Transport/trebucbd.ttf", 30);
    this->Fonts[FT_MENU] = new OpenUtility::CFontLoader("../content/Transport/trebuc.ttf", 30);

    // Fond et bandeau       
    this->Background = new OpenUtility::CQuad(0, 0, this->ScreenWidth, this->ScreenHeight*1.12, WHITE);
    this->Background->SetDefaultShaderMatrix(this->MVPMatrix, OpenUtility::CMat4x4<float>());

    this->Header = new OpenUtility::CQuad(0, 0, this->ScreenWidth, this->ScreenHeight*0.12, DARKGREEN);
    this->Header->SetDefaultShaderMatrix(this->MVPMatrix, OpenUtility::CMat4x4<float>());

    // Titre des écrans
    this->Title = new OpenUtility::C3DText(this->Fonts[FT_TITLE], 1.0, 1.0, 1.0, 1.0, 1.1);
    this->Title->SetAlignement(OpenUtility::CFontLoader::IFontEngine::EHAlignRight, OpenUtility::CFontLoader::IFontEngine::EVAlignTop);
    this->Title->UpdateText(this->Screens[this->Status.CurrentScreen]->GetTitle().c_str());

    this->MVPmatrixTitle = OpenUtility::CMat4x4<float>(this->MVPMatrix);
    this->MVPmatrixTitle *= OpenUtility::CMat4x4<float>().SetTranslate(this->ScreenWidth*0.99, this->ScreenHeight*0.03, 0);
    this->Title->SetDefaultShaderMatrix(MVPmatrixTitle);
    
    
    Logo = new OpenUtility::CTextureQuad("../content/Transport/logo.png", 3.5, 3.5);
    MVPmatrixLogo = OpenUtility::CMat4x4<float>(this->MVPMatrix);
    MVPmatrixLogo *= OpenUtility::CMat4x4<float>().SetTranslate(this->ScreenWidth*0.07, this->ScreenHeight*0.06, 0);

    this->LinesList->Init();
    
    for(int i=0; i<6;i++) {
        this->Screens[i]->Init();
    }
}

void CScreenTransport::UnInit()
{
    for(int i=0; i<6;i++) {
        this->Screens[i]->UnInit();
    }
    
    this->LinesList->UnInit();
    
    delete this->Background;
    delete this->Header;
    delete this->Logo;
    delete this->Title;
    
    delete[] this->Fonts;
}

bool CScreenTransport::PreRender()
{
    bool mustRender = false;

	// Premier affichage
	static bool firstInit = true;
	if (firstInit) {
		firstInit = false;
		mustRender = true;
	}
    
    // Changement d'écran
    static EScreenTransport previousScreen = this->Status.CurrentScreen;
    if (previousScreen != this->Status.CurrentScreen) {
        previousScreen = this->Status.CurrentScreen;
        this->Title->UpdateText(this->Screens[this->Status.CurrentScreen]->GetTitle().c_str());
        mustRender = true;
    }
    
    static bool changed = false;
    static unsigned long long start = pClass->GetTimeUnit();
    unsigned long long now = pClass->GetTimeUnit();
    if (!changed && (now - start) > 10000) {
        changed = true;
        this->Status.PreviousScreen = this->Status.CurrentScreen;
        this->Status.CurrentLine = "c6";
        this->Status.CurrentScreen = ST_SCHEDULE;
    }
    
    mustRender = mustRender || this->LinesList->PreRender();
    mustRender = mustRender || this->Screens[this->Status.CurrentScreen]->PreRender();
    
	return mustRender;
}

void CScreenTransport::Render()
{
    glDisable(GL_DEPTH_TEST);
    
    Background->Draw();
    Header->Draw();
    
    pClass->GetGlobalShader().UseProgram();
	glUniformMatrix4fv(pClass->GetGlobalShader()["u_MVPmatrix"], 1, GL_FALSE, MVPmatrixLogo.GetMatrix());
    
    Logo->AttachAttribToData(pClass->GetGlobalShader()["vPos"], pClass->GetGlobalShader()["vNorm"], pClass->GetGlobalShader()["vTexCoord"]);
    Logo->Draw();
    
    Title->Draw();
    
    this->LinesList->Render();
    this->Screens[this->Status.CurrentScreen]->Render();
    
    glEnable(GL_DEPTH_TEST);
}
