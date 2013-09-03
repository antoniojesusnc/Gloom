package principal
{
	import flash.ui.Mouse;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.external.ExternalInterface;

	
	/**
	 * ...
	 * @author Rubén Mulero
	 */
	public class MenuManager extends MovieClip 
	{
		var myCursor:Sprite;

		public function MenuManager() {
			init();
			
		}
		
		function childAddedToStage ( event:Event ) : void
		{
			 this.setChildIndex(this.getChildAt(getChildIndex(myCursor)), (this.numChildren-1));
		}
		
	//////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////  METHODS  //////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////
		
		function init()
		{
			//hide the mouse to put our own mouse cursor
			Mouse.hide();

			//load the custom mouse cursor
			myCursor = new CustomMouse();
			myCursor.mouseEnabled = false;
			myCursor.visible = false;
			//myCursor.setChildIndex(myCursor, numChildren - 1);
			
			// add the cursor  to the scene
			addChild(myCursor);
			//this.setChildIndex(this.getChildAt(getChildIndex(myCursor)), (this.numChildren-1));
			// respond to mouse move events
			stage.addEventListener(MouseEvent.MOUSE_MOVE, mouseMoveHandler);
			stage.addEventListener(Event.MOUSE_LEAVE, mouseLeaveHandler);
			stage.addEventListener ( Event.ADDED , childAddedToStage );
			
		}
		
	//////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////
		
		function mouseHide() {
			myCursor.visible = false;
		}
		
		function mouseShow() {
			myCursor.visible = true;
		}
		
	//////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////  LISTENERS  ////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////
		
		function mouseMoveHandler(evt:MouseEvent):void
		{
			// whenever the mouse moves, place the cursor in the same spot
			myCursor.visible = true;
			myCursor.x = evt.stageX;
			myCursor.y = evt.stageY;
		}

		function mouseLeaveHandler(evt:Event):void
		{
			myCursor.visible = false;
		}

	}
}