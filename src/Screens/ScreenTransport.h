//coding: utf-8
#ifndef _ScreenTransport_h
	#define _ScreenTransport_h

#include <string>
#include <vector>
#include <map>
#include "Transport/Structures.h"
#include "Screen.h"
#include "Transport/LinesList.h"

class CScreenTransport;
class CLinesList;

class CSubScreenTransport : public IRenderingObject
{
public:
    CSubScreenTransport(IRenderingObjectComm *obj, CScreenTransport* CommonScreen) : IRenderingObject(obj) {this->CommonScreen = CommonScreen;};
    CSubScreenTransport(const CSubScreenTransport &obj) : IRenderingObject(obj) {};
    CSubScreenTransport& operator=(const CSubScreenTransport &obj) {IRenderingObject::operator=(obj); return(*this);};
    ~CSubScreenTransport() {};
    
    virtual void Init() = 0;
	virtual void UnInit() = 0;
	virtual bool PreRender() = 0;
	virtual void Render() = 0;
    
    virtual std::string GetTitle() = 0;

protected:
    CScreenTransport* CommonScreen;
};

class CScreenTransport : public IScreen
{
public:
	CScreenTransport(IRenderingObjectComm *obj,OpenUtility::ITextureQuad *image=NULL);
	CScreenTransport(const CScreenTransport &obj);    
	virtual ~CScreenTransport();
	CScreenTransport& operator=(const CScreenTransport &obj);
    
	void Init();
	void UnInit();
	bool PreRender();
	void Render();
    
    inline int GetScreenPixelWidth() {return 1920;} // TODO : Remplacer par le contexte OpenGL
    inline int GetScreenPixelHeight() {return 1080;} // TODO : Remplacer par le contexte OpenGL
    inline int GetTaskbarHeight() {return 108;} // TODO : Remplacer par la vraie valeur
    
    inline float GetScreenWidth() {return this->ScreenWidth;}
    inline float GetScreenHeight() {return this->ScreenHeight;}
    inline OpenUtility::CMat4x4<float> GetMVPMatrix() {return this->MVPMatrix;}
    inline STransportStatus* GetStatus() {return &(this->Status);}
    inline OpenUtility::CFontLoader* GetFont(EFontTransport font) {return this->Fonts[font];}
    
    // Définition des couleurs
    static const OpenUtility::RGBAd WHITE; 
    static const OpenUtility::RGBAd BLACK; 
    static const OpenUtility::RGBAd DARKGREEN;
    static const OpenUtility::RGBAd GREY;
    
private:
    // ModelView général
    OpenUtility::CMat4x4<float> MVPMatrix;
    
    // Largeur et hauteur de l'écran (taskbar exclue)
    float ScreenWidth;
    float ScreenHeight;
    
    // Ecrans du module
    CSubScreenTransport* Screens[6];    
    
    // Polices
    OpenUtility::CFontLoader* Fonts[3];
    
    // Fond et bandeau    
    OpenUtility::CQuad* Background;
    OpenUtility::CQuad* Header;
    
    // Titre des écrans
    OpenUtility::C3DText* Title;
    OpenUtility::CMat4x4<float> MVPmatrixTitle;
    
    // Logo TAN
    OpenUtility::CTextureQuad* Logo;
    OpenUtility::CMat4x4<float> MVPmatrixLogo;    
    
    // Liste des lignes
    CLinesList* LinesList;
    
    
    // Etat courant du module
    STransportStatus Status;      
    
    // Lien vers le serveur
//    TransportFetcher Fetcher;
    
    // Liens vers les données
    std::string LogoPath;
	std::string MapButtonPath;
    
//    std::map<std::string, CLine> Lines;
	std::vector<std::string> NearLines;	
};

#endif
