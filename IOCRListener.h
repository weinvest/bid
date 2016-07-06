#pragma once

class CRecognizer;
class IOCRListener
{
public:
	virtual ~IOCRListener(void) {}

    virtual void OnNewRecognizer(const std::string& name, CRecognizer* pRecognizer) = 0;
	virtual void OnDelRecognizer(const std::string& name, CRecognizer* pRecognizer) = 0;
};

