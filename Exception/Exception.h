namespace UltimaAPI
{
	class BasicException;

	using Exception = BasicException;
}

class  UltimaAPI::BasicException
{
	__int64 code;
	const wchar_t* message;
public:

	BasicException() : message(L""), code(-1) 
	{ 
	
	}
	BasicException(const BasicException& e) : message(e.message), code(e.code)
	{ 
	
	}
	BasicException(const wchar_t* m) : message(m), code(-1) 
	{ 
	
	}
	BasicException(const wchar_t* m, __int64 c) : message(m), code(c) 
	{
	
	}

	virtual ~BasicException()
	{

	}
	virtual __int64 GetCode()
	{
		return code; 
	}
	virtual const wchar_t* GetMessage()
	{
		return message; 
	}
};
