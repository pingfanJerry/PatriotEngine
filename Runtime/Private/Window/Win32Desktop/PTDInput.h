﻿#ifndef _PT_PRIVATE_WINDOW_DInput_H_
#define _PT_PRIVATE_WINDOW_DInput_H_ 1

#include <sdkddkver.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <combaseapi.h>

class DECLSPEC_UUID("25E609E4-B259-11CF-BFC7-444553540000")
DirectInput8;

HRESULT STDMETHODCALLTYPE NoRegCoCreate(wchar_t const*dllName, REFCLSID rclsid, REFIID riid, void **ppv);

typedef struct DIDEVICEINSTANCEW
{
	DWORD   dwSize;
	GUID    guidInstance;
	GUID    guidProduct;
	DWORD   dwDevType;
	WCHAR   tszInstanceName[MAX_PATH];
	WCHAR   tszProductName[MAX_PATH];
	GUID    guidFFDriver;
	WORD    wUsagePage;
	WORD    wUsage;
} DIDEVICEINSTANCEW;

typedef BOOL(PASCAL *LPDIENUMDEVICESCALLBACKW)(DIDEVICEINSTANCEW const *, LPVOID);

struct IDirectInputDevice8W;

typedef BOOL(PASCAL *LPDIENUMDEVICESBYSEMANTICSCBW)(DIDEVICEINSTANCEW const *, IDirectInputDevice8W **, DWORD, DWORD, LPVOID);

typedef BOOL(PASCAL *LPDICONFIGUREDEVICESCALLBACK)(IUnknown *, LPVOID);

struct DIACTIONFORMATW;

struct DICONFIGUREDEVICESPARAMSW;

extern DWORD const DIRECTINPUT_VERSION;

extern DWORD const DI8DEVCLASS_GAMECTRL;

extern DWORD const DIENUM_CONTINUE;

extern DWORD const DIEDFL_ATTACHEDONLY;

MIDL_INTERFACE("BF798031-483A-4DA2-AA99-5D64ED369700")
IDirectInput8W : public IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE CreateDevice(REFGUID, IDirectInputDevice8W **, IUnknown *) = 0;
	virtual HRESULT STDMETHODCALLTYPE EnumDevices(DWORD, LPDIENUMDEVICESCALLBACKW, LPVOID, DWORD) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetDeviceStatus(REFGUID) = 0;
	virtual HRESULT STDMETHODCALLTYPE RunControlPanel(HWND, DWORD) = 0;
	virtual HRESULT STDMETHODCALLTYPE Initialize(HINSTANCE, DWORD) = 0;
	virtual HRESULT STDMETHODCALLTYPE FindDevice(REFGUID, LPCWSTR, LPGUID) = 0;
	virtual HRESULT STDMETHODCALLTYPE EnumDevicesBySemantics(LPCWSTR, DIACTIONFORMATW *, LPDIENUMDEVICESBYSEMANTICSCBW, LPVOID, DWORD) = 0;
	virtual HRESULT STDMETHODCALLTYPE ConfigureDevices(LPDICONFIGUREDEVICESCALLBACK, DICONFIGUREDEVICESPARAMSW *, DWORD, LPVOID) = 0;
};

struct DIDEVCAPS;

struct DIDEVICEOBJECTINSTANCEW;

typedef BOOL(PASCAL *LPDIENUMDEVICEOBJECTSCALLBACKW)(DIDEVICEOBJECTINSTANCEW const *, LPVOID);

typedef struct DIPROPHEADER
{
	DWORD   dwSize;
	DWORD   dwHeaderSize;
	DWORD   dwObj;
	DWORD   dwHow;
} DIPROPHEADER;

typedef struct DIPROPDWORD
{
	DIPROPHEADER diph;
	DWORD   dwData;
} DIPROPDWORD;

struct DIDEVICEOBJECTDATA;

struct DIOBJECTDATAFORMAT;

struct DIDATAFORMAT;

struct DIDEVICEINSTANCEW;

struct DIEFFECT;

struct IDirectInputEffect;

struct DIEFFECTINFOW;

typedef BOOL(PASCAL * LPDIENUMEFFECTSCALLBACKW)(DIEFFECTINFOW const *, LPVOID);

struct DIEFFECTINFOW;

typedef BOOL(PASCAL *LPDIENUMCREATEDEFFECTOBJECTSCALLBACK)(IDirectInputEffect *, LPVOID);

struct DIEFFESCAPE;

struct DIFILEEFFECT;

typedef BOOL(PASCAL *LPDIENUMEFFECTSINFILECALLBACK)(DIFILEEFFECT const *, LPVOID);

struct DIFILEEFFECT;

struct DIDEVICEIMAGEINFOHEADERW;

extern GUID const &DIPROP_DEADZONE;

extern DWORD const DIPH_BYOFFSET;

MIDL_INTERFACE("54D41081-DC15-4833-A41B-748F73A38179")
IDirectInputDevice8W : public IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE GetCapabilities(DIDEVCAPS *) = 0;
	virtual HRESULT STDMETHODCALLTYPE EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKW, LPVOID, DWORD) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetProperty(REFGUID, DIPROPHEADER *) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetProperty(REFGUID, DIPROPHEADER const *) = 0;
	virtual HRESULT STDMETHODCALLTYPE Acquire() = 0;
	virtual HRESULT STDMETHODCALLTYPE Unacquire() = 0;
	virtual HRESULT STDMETHODCALLTYPE GetDeviceState(DWORD, LPVOID) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetDeviceData(DWORD, DIDEVICEOBJECTDATA *, LPDWORD, DWORD) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetDataFormat(DIDATAFORMAT const *) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetEventNotification(HANDLE) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetCooperativeLevel(HWND, DWORD) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetObjectInfo(DIDEVICEOBJECTINSTANCEW *, DWORD, DWORD) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetDeviceInfo(DIDEVICEINSTANCEW *) = 0;
	virtual HRESULT STDMETHODCALLTYPE RunControlPanel(HWND, DWORD) = 0;
	virtual HRESULT STDMETHODCALLTYPE Initialize(HINSTANCE, DWORD, REFGUID) = 0;
	virtual HRESULT STDMETHODCALLTYPE CreateEffect(REFGUID, DIEFFECT const *, IDirectInputEffect **, LPUNKNOWN) = 0;
	virtual HRESULT STDMETHODCALLTYPE EnumEffects(LPDIENUMEFFECTSCALLBACKW, LPVOID, DWORD) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetEffectInfo(DIEFFECTINFOW *, REFGUID) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetForceFeedbackState(LPDWORD) = 0;
	virtual HRESULT STDMETHODCALLTYPE SendForceFeedbackCommand(DWORD) = 0;
	virtual HRESULT STDMETHODCALLTYPE EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK, LPVOID, DWORD) = 0;
	virtual HRESULT STDMETHODCALLTYPE Escape(DIEFFESCAPE *) = 0;
	virtual HRESULT STDMETHODCALLTYPE Poll() = 0;
	virtual HRESULT STDMETHODCALLTYPE SendDeviceData(DWORD, DIDEVICEOBJECTDATA const *, LPDWORD, DWORD) = 0;
	virtual HRESULT STDMETHODCALLTYPE EnumEffectsInFile(LPCWSTR, LPDIENUMEFFECTSINFILECALLBACK, LPVOID, DWORD) = 0;
	virtual HRESULT STDMETHODCALLTYPE WriteEffectToFile(LPCWSTR, DWORD, DIFILEEFFECT *, DWORD) = 0;
	virtual HRESULT STDMETHODCALLTYPE BuildActionMap(DIACTIONFORMATW *, LPCWSTR, DWORD) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetActionMap(DIACTIONFORMATW *, LPCWSTR, DWORD) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetImageInfo(DIDEVICEIMAGEINFOHEADERW *) = 0;
};

//Another Possible Choice: Steam Input?
//https://partner.steamgames.com/doc/api/isteaminput
//TestAppID=480

enum DICONTROLLERTYPE
{
	DICT_UNKNOWN = 0,
	DICT_XBOXONE = 1,
	DICT_PS4 = 2
};

DICONTROLLERTYPE WINAPI DIGetControllerTypeForGUID(REFGUID rguidProduct);

//Xbox One Wireless Controller
//https://support.xbox.com/en-US/xbox-one/accessories/xbox-one-wireless-controller

extern DIDATAFORMAT const c_dfDIXBOXONECONTROLLER;

struct DIXBOXONESTATE
{
	DWORD DPad; //D-Pad(Directional Pad) //None(-1) Up(0) Right(9000) Down(18000) Left(27000)
	BYTE A;
	BYTE B;
	BYTE X;
	BYTE Y;
	BYTE LB; //LB(Left Bumper)
	BYTE RB; //RB(Right Bumper)
	BYTE View;
	BYTE Menu;
	DWORD LTRT; //LT(Left Trigger) And RT(Right Trigger) //LT(65408)-None(32767)-RT(128)
	DWORD L_X; //L(Left Stick)_X(AxisX)
	DWORD L_Y; //L(Left Stick)_Y(AxisY)
	DWORD R_X; //L(Left Stick)_X(AxisX)
	DWORD R_Y; //L(Left Stick)_Y(AxisY)
	//The Xbox Button Is InVisible To Applications
};

//DUALSHOCK™4 wireless controller
//https://manuals.playstation.net/document/en/ps4/basic/pn_controller.html

extern DIDATAFORMAT const c_dfDIPS4CONTROLLER;

struct DIPS4STATE
{
	DWORD DPad; //D-Pad(Directional Pad) //None(-1) Up(0) Right(9000) Down(18000) Left(27000)
	BYTE A;
	BYTE B;
	BYTE X;
	BYTE Y;
	BYTE LB; //LB(Left Bumper)
	BYTE RB; //RB(Right Bumper)
	BYTE Share;
	BYTE Options;
	DWORD LT; //LT(Left Trigger)
	DWORD RT; //RT(Right Trigger)
	DWORD L_X; //L(Left Stick)_X(AxisX)
	DWORD L_Y; //L(Left Stick)_Y(AxisY)
	DWORD R_X; //L(Left Stick)_X(AxisX)
	DWORD R_Y; //L(Left Stick)_Y(AxisY)
	BYTE PS4;
	BYTE _1;
	BYTE _2;
	BYTE _3;
};

#endif