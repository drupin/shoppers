(function ($) {
    $.fn.imageEnlarge = function (options) {
			//Set the default values, use comma to separate the settings, example:
			var defaults = {
				multiple: 1.2,
				mouseInSpeed : 200,
				mouseOutSpeed : 400,
				zindex : 600,
				topOffset : 0,
				leftOffset : 0
			}
				
			var options =  $.extend(defaults, options);

			return this.each(function() {
				var $items = $(this).find('img'),
						$position = $items.position(),
						$iH = $items.outerHeight(true),
						$iW = $items.outerWidth(true),
						iHH = $iH * options.multiple,
						iHW = $iW * options.multiple,
						mT = ( iHH - $iH ) / 2,
						mL = ( iHW - $iW ) / 2,
						pT = $position.top - mT,
						pL = $position.left - mL;
			$(this).hover(function() {
				$items.addClass('iehover').css({'z-index' : options.zindex, 'position' : 'absolute'}).stop().animate({
					//marginTop: '-' + mT + 'px',
					//marginLeft: '-' + mL + 'px',
					top: pT + parseInt(options.topOffset),
					left: pL + parseInt(options.leftOffset),
					width: iHW,
					height: iHH
				}, options.mouseInSpeed);
			}, function() {
				$items.removeClass('iehover').css({'z-index' : options.zindex, 'position' : 'static'}).stop().animate({
					//marginTop: '0',
					//marginLeft: '0',
					top: $position.top,
					left: $position.left,
					width: $iW,
					height: $iH
				}, options.mouseOutSpeed);
			});
	});
};
})(jQuery);