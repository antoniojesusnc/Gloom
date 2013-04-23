/**
@file PlayerController.cpp

Contiene la implementaci�n de la clase CPlayerController. Se encarga de
recibir eventos del teclado y del rat�n y de interpretarlos para
mover al jugador.

@see GUI::CPlayerController

@author David Llans�
@date Agosto, 2010
*/

#include "PlayerController.h"
#include "InputManager.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Messages/Message.h"
#include "Logic/Entity/Components/AvatarController.h"
#include "Logic/Entity/Components/WeaponsManager.h"
#include "Logic/Entity/Components/ArrayGraphics.h"
#include "Logic/Entity/Components/AvatarController.h"

#include "Logic/Messages/MessageChangePlayerClass.h"
#include "Logic/Messages/MessageChangeWeapon.h"
#include "Logic/Messages/MessageControl.h"
#include "Logic/Messages/MessageMouse.h"

#include "Logic/Messages/MessageCameraOffset.h"

#include <cassert>
#include <memory>

//PRUEBAS,QUITAR LUEGO el INCLUDE SIGUIENTE
#include "Audio/Server.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Logic/Messages/MessageDamaged.h"

//MENSAJE PARA DEBBUG
#include "Logic/Messages/MessageHudDebug.h"

#define TURN_FACTOR_X 0.001f
#define TURN_FACTOR_Y 0.001f

namespace Input {

	CPlayerController::CPlayerController() : _controlledAvatar(0)
	{

	} // CPlayerController

	//--------------------------------------------------------

	CPlayerController::~CPlayerController()
	{
		deactivate();

	} // ~CPlayerController

	//--------------------------------------------------------

	void CPlayerController::activate()
	{		
		CInputManager::getSingletonPtr()->addKeyListener(this);
		CInputManager::getSingletonPtr()->addMouseListener(this);
		_time=clock();

		_strafingRight=false;
		_strafingLeft=false;
		_unpressLeft=false;
		_unpressRight=false;

		//Inicializacion variables de control ( salto, salto lateral, concatenacion salto lateral y rebote )
		_jumpLeft=0;
		_jumpRight=0;
		_timeSideJump=0;

		_unpressRight=false;
		_unpressLeft=false;
		_readySideJumpLeft=false;
		_readySideJumpRight=false;
		_dontCountUntilUnpress=false;

		_maxTimeSideJump=300;

	} // activate

	//--------------------------------------------------------

	void CPlayerController::deactivate()
	{
		CInputManager::getSingletonPtr()->removeKeyListener(this);
		CInputManager::getSingletonPtr()->removeMouseListener(this);
	} // deactivate

	//--------------------------------------------------------

	bool CPlayerController::keyPressed(TKey key)
	{						
		if(_controlledAvatar)
		{
			if(key.keyId == Input::Key::NUMBER1 || key.keyId == Input::Key::NUMBER2 || key.keyId == Input::Key::NUMBER3 || key.keyId == Input::Key::NUMBER4 || 
				key.keyId == Input::Key::NUMBER5 || key.keyId == Input::Key::NUMBER6 || key.keyId == Input::Key::NUMBER7 || key.keyId == Input::Key::NUMBER8){
				
					std::shared_ptr<Logic::CMessageChangeWeapon> message = std::make_shared<Logic::CMessageChangeWeapon>();							
					
					switch(key.keyId) {
						case Input::Key::NUMBER1: {
							message->setWeapon(0);
							break;
						}
						case Input::Key::NUMBER2: {
							message->setWeapon(1);
							break;
						}
						case Input::Key::NUMBER3: {
							message->setWeapon(2);
							break;
						}
						case Input::Key::NUMBER4: {
							message->setWeapon(3);
							break;
						}
						case Input::Key::NUMBER5: {
							message->setWeapon(4);
							break;
						}
						case Input::Key::NUMBER6: {
							message->setWeapon(5);
							break;
						}
						case Input::Key::NUMBER7: {
							Vector3 position = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("Jumper1")->getPosition();
							Audio::CServer::getSingletonPtr()->playSound3D("media/audio/plasma.wav", "cosa", position, false);
							
							std::shared_ptr<Logic::CMessageDamaged> damage = std::make_shared<Logic::CMessageDamaged>();
							damage->setDamage(100);
							damage->setEnemy(Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("Jumper1"));
							
							_controlledAvatar->emitMessage(damage);
							break;
						}
						case Input::Key::NUMBER8: {
							Audio::CServer::getSingletonPtr()->mute();
							break;
						}
					}

					_controlledAvatar->emitMessage(message);	
			}
			else{
				std::shared_ptr<Logic::CMessageControl> m = std::make_shared<Logic::CMessageControl>();
				std::shared_ptr<Logic::CMessageHudDebug> m2 = std::make_shared<Logic::CMessageHudDebug>();

				switch(key.keyId)
				{
				case Input::Key::Q:
					m->setType(Logic::Control::USE_PRIMARY_SKILL);
					break;
				case Input	::Key::E:
					m->setType(Logic::Control::USE_SECONDARY_SKILL);
					break;
				case Input::Key::W:
					m->setType(Logic::Control::WALK);
					break;
				case Input::Key::S:
					m->setType(Logic::Control::WALKBACK);
					break;
				case Input::Key::A:
					m->setType(Logic::Control::STRAFE_LEFT);
					_strafingLeft=true;
					break;
				case Input::Key::D:
					m->setType(Logic::Control::STRAFE_RIGHT);
					_strafingRight=true;
					break;
				case Input::Key::SPACE:
					m->setType(Logic::Control::JUMP);
					break;
				case Input::Key::O:
					_controlledAvatar->emitMessage(m2);
					break;
				case Input::Key::ESCAPE:// esto debe desaparecer en el futuro
					return false;
					break;

				case Input::Key::T:
					{
						std::shared_ptr<Logic::CMessageCameraOffset> m3 = std::make_shared<Logic::CMessageCameraOffset>();
						m3->setOffsetTimer(100.0f);
						//_controlledAvatar->emitMessage(m3); 
								 
						Logic::CEntity * camera = Logic::CServer::getSingletonPtr()->getMap()->getEntityByType("Camera");
						camera->emitMessage(m3);
					}
					break;

				default:
					return true;
				}
				if (key.keyId != Input::Key::O){
					//Tomamos el tiempo actual, calculamos la diferencia y actualizamos para la siguiente comprobacion
					unsigned int actualTime = clock();
					unsigned int diffTime= (_time-actualTime)%1000; //Diferencia de tiempo en msecs
					_time=actualTime;

			
					//Antes de emitir el tipico mensaje vemos si se trata de un doble "click" hacia algun lado
					
					//Control del tiempo para el salto lateral(al contar aqui cuando se activa no se cuenta el primer tick)
					if(_jumpLeft!=0 || _jumpRight!=0){
						_timeSideJump+=diffTime;
					}
					else{
						_timeSideJump=0;
					}

					//Controlamos cuando soltamos la tecla para hacer que la siguiente vez se active el salto
					if(_unpressLeft && _jumpLeft==1 && !_dontCountUntilUnpress){
						_readySideJumpLeft=true;
						_readySideJumpRight=false;
						_jumpRight=0;
						_unpressLeft=false;
					}
					else if(_unpressRight && _jumpRight==1 && !_dontCountUntilUnpress){
						_readySideJumpRight=true;
						_readySideJumpLeft=false;
						_jumpLeft=0;
						_unpressRight=false;
					}
					//Cuando soltemos la segunda presi�n entonces empezamos el conteo de presiones otra vez
					else if((_unpressRight || _unpressLeft) && _dontCountUntilUnpress){
						_dontCountUntilUnpress=false;
						_unpressRight=false;
						_unpressLeft=false;
						_jumpRight=0;
						_jumpLeft=0;
						_readySideJumpLeft=false;
						_readySideJumpRight=false;
						_timeSideJump=0;
					}
					else if(_unpressRight || _unpressLeft){
						_unpressRight=false;
						_unpressLeft=false;
					}

					//Izquierda/Derecha
					if(_strafingLeft || _strafingRight)
					{
						//Si se presionaron ambas teclas
						if(_strafingRight && _strafingLeft){
							_timeSideJump=0;
							_jumpRight=0;
							_jumpLeft=0;
							_readySideJumpLeft=false;
							_readySideJumpRight=false;
						}
						//Si se presion� la izq , el contador esta a 0 y puedo contar || o el salto izq esta listo y le di a la izq
						else if((_strafingLeft && _jumpLeft==0 && !_dontCountUntilUnpress) || (_readySideJumpLeft && _strafingLeft)){
							if(_jumpRight!=0)
								_timeSideJump=0;
							_jumpLeft++;
							_jumpRight=0;
							_readySideJumpLeft=false;
							_readySideJumpRight=false;
						}
						//contrario al de arriba
						else if((_strafingRight  && _jumpRight==0 && !_dontCountUntilUnpress) || (_readySideJumpRight && _strafingRight)){
							if(_jumpLeft!=0)
								_timeSideJump=0;
							_jumpRight++;
							_jumpLeft=0;
							_readySideJumpLeft=false;
							_readySideJumpRight=false;
						}

						//Si se activo el salto lateral hacia algun lado, est� dentro del tiempo
						if((_jumpRight==2 || _jumpLeft==2) && _timeSideJump<_maxTimeSideJump){ 
							if(_jumpRight==2){
								m->setType(Logic::Control::SIDEJUMP_RIGHT);
							}
							else{
								m->setType(Logic::Control::SIDEJUMP_LEFT);
							}
							_dontCountUntilUnpress=true;
							_timeSideJump=0;
							_readySideJumpRight=false;
							_readySideJumpLeft=false;
							_jumpRight=0;
							_jumpLeft=0;
							
						}
						//Si se pas� el tiempo reseteo
						else if(_timeSideJump>_maxTimeSideJump){
							_timeSideJump=0;
							_jumpLeft=0;
							_jumpRight=0;
							_readySideJumpRight=false;
							_readySideJumpLeft=false;
						}
					}

					_controlledAvatar->emitMessage(m);
				}

				return true;
			}
		}
		return true;

	} // keyPressed

	//--------------------------------------------------------

	bool CPlayerController::keyReleased(TKey key)
	{
		if(_controlledAvatar)
		{
			std::shared_ptr<Logic::CMessageControl> m = std::make_shared<Logic::CMessageControl>();
			switch(key.keyId)
			{
			case Input::Key::Q:
				m->setType(Logic::Control::STOP_PRIMARY_SKILL);
				break;
			case Input::Key::E:
				m->setType(Logic::Control::STOP_SECONDARY_SKILL);
				break;
			case Input::Key::W:
				m->setType(Logic::Control::STOP_WALK);
				break;
			case Input::Key::S:
				m->setType(Logic::Control::STOP_WALKBACK);
				break;
			case Input::Key::A:
				m->setType(Logic::Control::STOP_STRAFE_LEFT);
				_strafingLeft = false;
				_unpressLeft = true;
				break;
			case Input::Key::D:
				m->setType(Logic::Control::STOP_STRAFE_RIGHT);
				_strafingRight = false;
				_unpressRight = true;
				break;
			case Input::Key::ESCAPE:// esto debe desaparecer en el futuro
					std::cout << "escape pulsado" << std::endl;
					return false;
			default:
				return true;
			}
			_controlledAvatar->emitMessage(m);
			return true;
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CPlayerController::mouseMoved(const CMouseState &mouseState)
	{
		if(_controlledAvatar)
		{
			std::shared_ptr<Logic::CMessageMouse> m = std::make_shared<Logic::CMessageMouse>();
			m->setType(Logic::Control::MOUSE);
			float mouse[]={-(float)mouseState.movX * TURN_FACTOR_X,-(float)mouseState.movY * TURN_FACTOR_Y};
			m->setMouse(mouse);
			_controlledAvatar->emitMessage(m);
			return true;
		}
		return true;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CPlayerController::mousePressed(const CMouseState &mouseState)
	{
		if(_controlledAvatar)
		{
			std::shared_ptr<Logic::CMessageControl> m = std::make_shared<Logic::CMessageControl>();
			switch(mouseState.button)
			{
			case Input::Button::LEFT:
				m->setType(Logic::Control::LEFT_CLICK);
				break;
			case Input::Button::RIGHT:
				m->setType(Logic::Control::RIGHT_CLICK);
				break;
			case Input::Button::MIDDLE:
				m->setType(Logic::Control::MIDDLE_CLICK);
				break;
			case Input::Button::BUTTON_3:
				m->setType(Logic::Control::BUTTON3_CLICK);
				break;
			
			default:
				return true;
			}
			_controlledAvatar->emitMessage(m);
			return true;
		}
		return true;

	} // mousePressed

	//--------------------------------------------------------
	
	bool CPlayerController::mouseReleased(const CMouseState &mouseState)
	{
		
		if(_controlledAvatar)
		{
			std::shared_ptr<Logic::CMessageControl> m = std::make_shared<Logic::CMessageControl>();
			switch(mouseState.button)
			{
			case Input::Button::LEFT:
				m->setType(Logic::Control::UNLEFT_CLICK);
				break;
			case Input::Button::RIGHT:
				m->setType(Logic::Control::UNRIGHT_CLICK);
				break;
			case Input::Button::MIDDLE:
				m->setType(Logic::Control::UNMIDDLE_CLICK);
				break;
			case Input::Button::BUTTON_3:
				m->setType(Logic::Control::UNBUTTON3_CLICK);
				break;
			
			default:
				return true;
			}
			_controlledAvatar->emitMessage(m);
			return true;
		}
		return false;

	} // mouseReleased

} // namespace Input
