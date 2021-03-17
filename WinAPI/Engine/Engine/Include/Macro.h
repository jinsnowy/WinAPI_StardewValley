#pragma once

#define SAFE_DELETE(p)			if(p)	{delete p; p = nullptr;}
#define SAFE_DELETE_ARRAY(p)	if(p)	{delete[] p; p = nullptr;}
#define SAFE_RELEASE(p)			if(p)	{p->Release(); p = nullptr;}

#define EXCEPT(MSG)		UserException(MSG, __LINE__, __FILE__)
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

#define DEFINITION_SINGLE(Type) Type* Type::INSTANCE = 0;
#define DECLARE_SINGLE(Type)	\
		private:\
			static Type * INSTANCE;\
			Type();\
			~Type();\
		public:\
			static Type* Instance()\
			{\
				if(INSTANCE == 0)\
				{\
					INSTANCE = new Type;\
				}\
				return INSTANCE;\
			}\
			void Release()\
			{\
				if(INSTANCE)\
				{\
					delete INSTANCE;\
					INSTANCE = nullptr;\
				}\
			}\
			Type(const Type&) = delete;\
			Type(const Type&&) = delete;\
			Type& operator=(const Type&) = delete;\
			Type& operator=(const Type&&) = delete;