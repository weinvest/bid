struct WindowArea
{
    1: required i32 left;
    2: required i32 right;
    3: required i32 top;
    4: required i32 bottom;
}

enum RecognizeResultConfidence
{
    UNKNOWN = 0,
    GUESS = 1,
    RECOGNIZE = 2
}

//-----------------------Font Center Scan-------------------------
const i32 RECAPTCHA_MESSAGE_FONT_CENTER_SCAN = 100000;
struct FontCenterScanRequest
{
    1: required binary image;
    2: required list<WindowArea> centerWindows;
}


struct FontCenterScanResponse
{
    1: required i32 status;
    2: optional string message;
    3: required list<string> results;
}

//-----------------------Font Center Scan Index-------------------------
const i32 RECAPTICHA_MESSAGE_FONT_CENTER_SCAN_INDEX = 100001
struct FontCenterScanIndexRequest
{
    1: required binary image;
    2: required list<WindowArea> centerWindows;
    3: required list<WindowArea> indexAreas;
    4: required string desiredIndexValue;
}

struct FontCenterScanIndexResponse
{
    1: required i32 status;
    2: optional string message;
    3: required list<string> results;
    4: required i32 guessId;
}

//----------------------Font Center Scan Index Feedback----------------
const i32 RECAPTICHA_MESSAGE_FONT_CENTER_SCAN_INDEX_FEEDBACK = 100002
struct FontCenterScanIndexFeedbackRequest
{
    1: required i32 guessId;
    2: required bool success;
}

struct FontCenterScanIndexFeedbackResponse
{
    1: required i32 status;
    2: optional string message;
}
