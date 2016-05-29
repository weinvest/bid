#include "StdAfx.h"
#include "InfoEngine.h"

void InfoEngine::Registe(IInfoListener* pListener)
{
	mListeners.insert(pListener);
}

void InfoEngine::UnRegiste(IInfoListener* pListener)
{
	mListeners.erase(pListener);
}

void InfoEngine::Start( void )
{

}

void InfoEngine::Stop( void )
{

}

void SetRect(size_t idx, CRect& rect, DataType dtype)
{
	auto& info = mInfoRects[idx];
	info.type = dtype;
    info.rect = rect;
	info.value = 0;
}

void InfoEngine::CollectData(size_t index)
{

}