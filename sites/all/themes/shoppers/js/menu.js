(function ($) {
	Drupal.behaviors.shoppersmenu = {
		attach: function(context, settings) {
			$('.view-shoppers-categories-menu', context).hoverIntent({
				over: function () {
					$(this).find('.view-grouping-content').slideDown('fast', 'easeInCubic');
				},
				out: function () {
					$(this).find('.view-grouping-content').slideUp('fast', 'easeInCubic');
				},
				timeout: 500
			});
			$('.view-shoppers-categories-menu .view-grouping-content .item-list').hover(function() {
				var t = $(this);
				clearTimeout(t.data('timeout'));
				t.siblings().find('ul').hide().stop(true,true);
				t.find('ul').animate({ width : 'show' }, 0); 
			}, function() {
				var e = $(this);
				e.data('timeout', setTimeout(function() {
						e.find('ul').stop(true,true).animate( { width: 'hide' }, 400);
					}, 700)
				); 
			});
		}
	};
})(jQuery);
