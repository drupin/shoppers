( function($) {
	$(document).ready(function() {
		$('select[name="theme_preset"]').change(function() {
			$('#'+$(this).val()+'-theme').show().siblings().hide();
		});
	});
})(jQuery);