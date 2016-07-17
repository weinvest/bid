#pragma once
#include <bitset>
#include <unordered_map>
#include <functional>
#include <memory>
#include <ostream>
#include "ScreenImage.h"

class TDBitArray 
{
public:
	TDBitArray( void )
		: mWeight(0)
		, mArea(TOP_INIT, LEFT_INIT, BOTTOM_INIT, RIGHT_INIT)
	{
	}

	size_t GetWidth() const { return mArea.Width() + 1; }
	size_t GetHeight() const { return mArea.Height() + 1; }
	size_t GetCount() const { return GetWidth() * GetHeight(); }
	size_t GetWeigt() const { return mWeight;  }

	bool Get(int width, int height) const;
	bool Get(int pos) const;
	bool Set(int width, int height);

	void UpdateArea(int width, int height);

	void Output(std::ostream& out);

	~TDBitArray(){}

	int ComputeDist(const TDBitArray& other, int* pBuffer);

	static int* CreateDistBuffer(void);
	static void FreeDistBuffer(int*& pBuffer);
	static int GetDist(int width, int height, int* pBuffer);
	static void SetDist(int width, int height, int dist, int* pBuffer);
private:
	bool GetImpl(int width, int height) const;

	static const int TOP_INIT = 100000;
	static const int LEFT_INIT = 100000;
	static const int BOTTOM_INIT = -1;
	static const int RIGHT_INIT = -1;

	static const size_t MAX_WIDTH = 128;
	static const size_t MAX_HEIGHT = 64;

	size_t mWeight;
	CRect mArea;
	std::bitset<MAX_WIDTH * MAX_HEIGHT> mData;
};

//class PixelAnyalizeResult
//{
//public:
//private:
//	std::vector<>
//};
void DoPixelAnyalize(CScreenImage& image, std::unordered_map<COLORREF, std::shared_ptr<TDBitArray>>& out);
void PixelAnyalizeAll(const std::string& path);