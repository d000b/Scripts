#pragma once;

// IDK why I really need a common destructor, but it seemed like it was for the correct removal of composite classes via Vector

namespace UltimaAPI
{
	class BasicDestructor;
}

class UltimaAPI::BasicDestructor
{
	BasicDestructor()  { }
	~BasicDestructor() { }
};
