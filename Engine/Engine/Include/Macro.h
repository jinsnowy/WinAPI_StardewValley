#pragma once

#define SAFE_DELETE(p)			if(p)	{delete p; p = nullptr;}
#define SAFE_DELETE_ARRAY(p)	if(p)	{delete[] p; p = nullptr;}
#define SAFE_RELEASE(p)			if(p)	{p->Release(); p = nullptr;}

#define KEYDOWN(key)	Input::Instance()->KeyDown(key)
#define KEYUP(key)		Input::Instance()->KeyUp(key)
#define KEYPRESS(key)	Input::Instance()->KeyPress(key)
#define MOUSECLIENTPOS  Input::Instance()->GetMouseClientPos()
#define MOUSEWORLDPOS   Input::Instance()->GetMouseWorldPos()
#define MOUSEMOVE       Input::Instance()->GetMouseMove()
#define MOUSE			Input::Instance()->GetMouse()
#define PLAYER			GameManager::Instance()->AccessPlayer()
#define GAMEWORLDTIME	GameManager::Instance()->GetWorldTime()
#define SCENECHANGE(X)  SceneManager::Instance()->NeedChangeScene(X)
#define SHOWCHECK(X)    SceneManager::Instance()->CheckShowMode(X)
#define DRAWGLYPH(hdc, c, x, y) ResourceManager::Instance()->DrawGlyph(hdc, c, x, y)
#define TRIGGER_CLICKEVENT(POS, TAG) CollisionManager::Instance()->AddCollidePoint(POS, TAG)
#define TRIGGER_RECTEVENT(POS, RECTA, TAG) CollisionManager::Instance()->AddCollideRect(POS, RECTA, TAG)

#define DEFINITION_SINGLE(Type) Type* Type::_instance = nullptr;
#define DECLARE_SINGLE(Type)\
		private:\
			static Type * _instance;\
			Type();\
			~Type();\
		public:\
			static Type* Instance()\
			{\
				if(_instance == 0)\
				{\
					_instance = new Type;\
				}\
				return _instance;\
			}\
			void Release()\
			{\
				if(_instance)\
				{\
					delete _instance;\
					_instance = nullptr;\
				}\
			}\
			Type(const Type&) = delete;\
			Type(const Type&&) = delete;\
			Type& operator=(const Type&) = delete;\
			Type& operator=(const Type&&) = delete;