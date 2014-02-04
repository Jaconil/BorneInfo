//coding: utf-8
#include "../ScreenTransport.h"
#include "ScrollBar.h"

CScrollBar::CScrollBar(IRenderingObjectComm *obj, IScrollable* scrollableContent) : 
    IRenderingObject(obj) 
{
    this->ScrollableContent = scrollableContent;
    this->UpArrow = NULL;
    this->DownArrow = NULL;
    this->Bar = NULL;
    
    this->CurrentScroll = 0.0;
    this->ScrollHeight = 1.0;
    
    this->MustRender = true;
}

void CScrollBar::Init()
{
    this->ScrollHeight = this->ScrollableContent->GetBoxHeight() / this->ScrollableContent->GetContentHeight();
    this->MustRender = true;
    
    this->Width = 0.023 * this->ScrollableContent->GetScreen()->GetScreenWidth();
    this->Height = -this->Width;
    
    this->TotalBarHeight = this->ScrollableContent->GetBoxHeight() - 2*this->Height - 2*this->Height/4.0; // Soustraction des flèches et des espacements
    
    // Calibrage du ModelView
    // 0,0 = Coin haut gauche de la scrollbar
    this->MVPmatrix = OpenUtility::CMat4x4<float>(this->ScrollableContent->GetMVPMatrix());
    this->MVPmatrix *= OpenUtility::CMat4x4<float>().SetTranslate(this->ScrollableContent->GetBoxWidth() + 0.014*this->ScrollableContent->GetScreen()->GetScreenWidth(), 0, 0);
    
    this->UpArrow = new OpenUtility::CTriangle(this->Width/2, 0, this->Width, this->Height, 0, this->Height, CScreenTransport::DARKGREEN);
    this->UpArrow->SetDefaultShaderMatrix(this->MVPmatrix, OpenUtility::CMat4x4<float>());
        
    this->DownArrow = new OpenUtility::CTriangle(0, this->ScrollableContent->GetBoxHeight() - this->Height, this->Width, this->ScrollableContent->GetBoxHeight() - this->Height, this->Width/2, this->ScrollableContent->GetBoxHeight(), CScreenTransport::DARKGREEN);
    this->DownArrow->SetDefaultShaderMatrix(this->MVPmatrix, OpenUtility::CMat4x4<float>());
    
    this->Bar = new OpenUtility::CQuad(0, 0, this->Width, this->TotalBarHeight, CScreenTransport::DARKGREEN);
    this->BarMatrix = OpenUtility::CMat4x4<float>(this->MVPmatrix);    
    this->Bar->SetDefaultShaderMatrix(this->BarMatrix, OpenUtility::CMat4x4<float>());
}

void CScrollBar::UnInit()
{
    delete this->UpArrow;
    delete this->DownArrow;
    delete this->Bar;
}

bool CScrollBar::PreRender()
{
    bool mustRender = this->MustRender;
    this->MustRender = false;
    
    this->ScrollHeight = this->ScrollableContent->GetBoxHeight() / this->ScrollableContent->GetContentHeight();    
    float barOffset = (this->TotalBarHeight - this->TotalBarHeight * this->ScrollHeight) * this->CurrentScroll + this->Height*5/4;
    
    this->BarMatrix = OpenUtility::CMat4x4<float>(this->MVPmatrix);  
    this->BarMatrix *= OpenUtility::CMat4x4<float>().SetTranslate(0, barOffset, 0);
    this->BarMatrix *= OpenUtility::CMat4x4<float>().SetScale(1, this->ScrollHeight, 1);
    this->Bar->SetDefaultShaderMatrix(this->BarMatrix, OpenUtility::CMat4x4<float>());
    
    return mustRender;
}

void CScrollBar::Render()
{
    this->UpArrow->Draw();
    this->DownArrow->Draw();
    this->Bar->Draw();
}

void CScrollBar::ScrollTo(float scroll)
{
    this->CurrentScroll = scroll;
    this->MustRender = true;
}
