#pragma once

class CRecognizer;
class IOCRListener
{
public:
	virtual ~IOCRListener(void) {}

    virtual void OnNewRecognizer(const CString& name, CRecognizer* pRecognizer) = 0;
	virtual void OnDelRecognizer(const CString& name, CRecognizer* pRecognizer) = 0;
};

