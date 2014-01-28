//coding: utf-8
#include "LinesList.h"

CLinesList::CLinesList(IRenderingObjectComm *obj, CScreenTransport* commonScreen) : 
    IRenderingObject(obj) 
{
    this->CommonScreen = commonScreen;
    
    this->Left = 0.0093 * this->CommonScreen->GetScreenWidth();
    this->Top = 0.15 * this->CommonScreen->GetScreenHeight();
    this->Right = 0.297 * this->CommonScreen->GetScreenWidth();
    this->Bottom = 1.0 * this->CommonScreen->GetScreenHeight();
}

void CLinesList::Init()
{
    this->Background = new OpenUtility::CQuad(this->Left, this->Top, this->Right, this->Bottom, CScreenTransport::DARKGREEN, CScreenTransport::DARKGREEN, CScreenTransport::DARKGREEN, CScreenTransport::DARKGREEN);
    this->Background->SetDefaultShaderMatrix(this->CommonScreen->GetMVPMatrix(), OpenUtility::CMat4x4<float>());
}

void CLinesList::UnInit()
{
    delete this->Background;
}

bool CLinesList::PreRender()
{
    bool mustRender = false;
    
    // Calculer hauteur listes
    // MAJ scrollbar
    // -> faire ça dans une méthode Update appelé à chaque changement d'écran ?
    // Penser à ajouter une matrice MV à l'objet, modifiée avec le scroll
    
    return mustRender;
}

void CLinesList::Render()
{
    this->Background->Draw();
    
    if (this->CommonScreen->GetStatus()->CurrentScreen == CScreenTransport::ST_HOME) {
        // Draw all lines
    } else {
        if (this->CommonScreen->GetStatus()->CurrentLine != "") {
            if (this->CommonScreen->GetStatus()->CurrentStop != "") {
                // Draw StopConnected
            } else {
                // Draw LineConnected
            }
        }
        
        // Draw near lines
        // Draw other lines
    }
}
