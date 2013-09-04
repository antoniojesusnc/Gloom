package MenuPrincipal
{
	import flash.events.MouseEvent;
	import flash.external.ExternalInterface;
	import principal.GloomButton;
	/**
	 * Button interface
	 * @author Rubén Mulero
	 */
	public class ExitButton extends GloomButton
	{
		public function ExitButton() 
		{
			super();;
			
		}
		
		
		protected override function onMouseClick(e:MouseEvent):void {
			ExternalInterface.call("exitClick");
		}
	}

}