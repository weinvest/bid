#include "Stdafx.h"
#include <fstream>
#include "PixelBitAnalyze.h"
bool TDBitArray::Get(int width, int height) const
{
	return GetImpl(mArea.left + width, mArea.top + height);
}

bool TDBitArray::GetImpl(int width, int height) const
{
	return width < 0 || height < 0 || width >= MAX_WIDTH || height >= MAX_HEIGHT ? 0 : mData[height * MAX_WIDTH + width];
}

bool TDBitArray::Get(int pos) const
{
	int width = pos % GetWidth();
	int height = pos / GetHeight();
	return Get(width, height);
}

bool TDBitArray::Set(int width, int height)
{
	//if (TOP_INIT == mArea.top || Get(width - 1, height - 1)
	//	|| Get(width, height - 1) || Get(width + 1, height - 1) || Get(width - 1, height))
	{
		mData.set(height * MAX_WIDTH + width, true);
		UpdateArea(width, height);
		++mWeight;

		return true;
	}

	return false;
}

void TDBitArray::UpdateArea(int width, int height)
{
	if (height < mArea.top)
	{
		mArea.top = height;
	}

	if (height > mArea.bottom)
	{
		mArea.bottom = height;
	}

	if (width < mArea.left)
	{
		mArea.left = width;
	}

	if (width > mArea.right)
	{
		mArea.right = width;
	}
}

void TDBitArray::Output(std::ostream& out)
{
	out << mWeight << "\n";
	size_t iPos = 0;
	for (size_t height = 0; height < GetHeight(); ++height)
	{
		for (size_t width = 0; width < GetWidth(); ++width)
		{
			out << Get(width, height);
		}
		out << "\n";
	}
}

int* TDBitArray::CreateDistBuffer(void)
{
	return new int[(MAX_WIDTH + 1) * (MAX_HEIGHT + 1)];
}

void TDBitArray::FreeDistBuffer(int*& pBuffer)
{
	delete[] pBuffer;
	pBuffer = nullptr;
}

int TDBitArray::GetDist(int width, int height, int* pBuffer)
{
	return pBuffer[height * MAX_WIDTH + width];
}

void TDBitArray::SetDist(int width, int height, int dist, int* pBuffer)
{
	pBuffer[height * MAX_WIDTH + width] = dist;
}

int TDBitArray::ComputeDist(const TDBitArray& other, int* pBuffer)
{
	int n = GetCount();
	int m = other.GetCount();
	
	for (int s = 0; s <= m; ++s)
	{
		SetDist(0, s, s, pBuffer);
	}

	for (int t = 1; t <= n; ++t)
	{
		SetDist(t, 0, t, pBuffer);
	}
	
	for (int i = 1; i <= n; ++i)
	{
		for (int j = 1; j <= m; ++j)
		{
			int substituteCost = 0;
			if (Get(i - 1) != other.Get(j - 1))
			{
				substituteCost = 1;
			}

			auto dist = std::min(GetDist(i - 1, j, pBuffer) + 1, GetDist(i, j - 1, pBuffer));
			dist = std::min(dist, GetDist(i - 1, j - 1, pBuffer) + substituteCost);
			SetDist(i, j, dist, pBuffer);
		}
	}
	return GetDist(n, m, pBuffer);
}

void DoPixelAnyalize(CScreenImage& image, std::unordered_map<COLORREF, std::shared_ptr<TDBitArray>>& out)
{
	size_t width = image.GetWidth();
	size_t height = image.GetHeight();

	for (size_t h = 0; h < height; ++h)
	{
		for (size_t w = 0; w < width; ++w)
		{
			COLORREF c = image.GetPixel(w, h);
			int feature = c == RGB(255,255,255) ? 0 : 1;
			if (0 != feature)
			{
				int r = GetRValue(c);
				int g = GetGValue(c);
				int b = GetBValue(c);

				std::shared_ptr<TDBitArray> pBits;
				for (auto& outPair : out)
				{
					int r1 = GetRValue(outPair.first);
					int g1 = GetGValue(outPair.first);
					int b1 = GetBValue(outPair.first);
					auto dist = (r1 - r) * (r1 - r) + (g1 - g) * (g1 - g) + (b1 - b) * (b1 - b);
					if (dist < 500)
					{
						pBits = outPair.second;
					}
				}

				if (nullptr == pBits)
				{
					pBits = std::make_shared<TDBitArray>();
					out.insert(std::make_pair(c, pBits));;
				}

				pBits->Set(w, h);
			}
		}
	}
}

void PixelAnyalizeAll(const std::string& mFontRootDir)
{
	return;
	CFileFind ff;
	std::string dir = mFontRootDir + "\\*.bmp";

	BOOL ret = ff.FindFile(CString(dir.c_str()));
	while (ret)
	{
		ret = ff.FindNextFile();
		if (!ff.IsDirectory())
		{
			KnowledgeT knowledge;
			int sepCount(0);

			CScreenImage image;
			image.Load(ff.GetFilePath());
#if 1			
			std::unordered_map<COLORREF, std::shared_ptr<TDBitArray>> pixelLayer;
			DoPixelAnyalize(image, pixelLayer);

			std::string outFileName = CT2A(ff.GetFilePath());
			outFileName += ".ana";
			std::ofstream layerOut(outFileName, std::ios_base::binary | std::ios_base::out);

			for (auto& p : pixelLayer)
			{
				if (p.second->GetWeigt() > 20)
				{
					p.second->Output(layerOut);
				}
			}
#else
			std::string outFileName = CT2A(ff.GetFilePath());
			outFileName += ".ana";
			std::ofstream layerOut(outFileName, std::ios_base::binary | std::ios_base::out);
			for (int h = 0; h < image.GetHeight(); ++h)
			{
				for (int w = 0; w < image.GetWidth(); ++w)
				{
					char s[64] = { '\0' };
					auto c = image.GetPixel(w, h);
					BYTE r = GetRValue(c);
					BYTE g = GetGValue(c);
					BYTE b = GetBValue(c);
					if (r == 255 && g == 255 && b == 255)
					{
						snprintf(s, 64, "(%2d,%2d,%2d)", 0, 0, 0);
					}
					else
					{
						snprintf(s, 64, "(%2d,%2d,%2d)", r/16, g/16, b/16);
					}
					layerOut << s;
				}
				layerOut << std::endl;
			}
#endif
		}
	}
}