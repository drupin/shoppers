jQuery(document).ready(function ($) {
	// we need this because Drupal Commerce has ajax script on product display page
	// without this the bubbling will failed
	Drupal.behaviors.vtCommerceImage = { 
	    attach: function() {
	      attach();
	    },
	    detach: function() {
	      $('.cloud-thumbnail').remove();
	    }
	}
	
	// attach for the first time  
	attach();
	
	function attach() {

	  var option = Drupal.settings.vt_commerce_image;
		var clouds = $('.cloud-zoom'),
		    parents = $('.vt-commerce-image-wrapper'), // your parent element
		    wrap = $('.wrap'),
		    z = 9000,
        cHeight = clouds.find('img').outerHeight(true),
        cWidth = clouds.find('img').outerWidth(true);
		    
		if (option.zoom == 1) {
		  clouds.CloudZoom();
		}

	  // initialization
    // This is very important steps because jQuery cloud js need the element
    // to be visible when intialized, so we cannot just plain hide the
    // large element, instead we use z-index to hide the element and 
    // show the first element on load
    
    // reorder the zindex so the first element will be always shown first
	  // hide all wrap except the first one;
    wrap.each(function() {
      $(this).css('z-index',  z ).hide();
      z--;	     
    }).eq(0).show();
    
    // set the large image wrapper height
    $('.vt-commerce-image-large').height(cHeight);
    
    // create the thumbnail wrapper so we can add thumbnail child here later on
    parents.css('position', 'relative').append('<div class="cloud-thumbnail"/>');
  
    // only build thumbnail if more than 1 images found
    if (clouds.length > 1) {
	    // cloning the large image to create a thumbnail navigation
	    clouds.each(function() {
	      var thumbClone = $(this).find('img').clone().width(option.previewWidth).height(option.previewHeight);
	      thumbClone.appendTo('.cloud-thumbnail').addClass('thumbsmall').wrap('<div class="cloud-thumbwrapper" />');      
	    });
	    
	    // declare the newly cloned thumb as a new variable
	    var thumbSmall = $('.thumbsmall');
	    
	    // function for the thumbnail to show the right large image
	    thumbSmall.each(function(){      
	      $(this).unbind('click').click(function() {
	        var index = thumbSmall.index(this);
	        wrap.stop().hide(0, function() { 
	          clouds.eq(index).parents('.wrap').show();
	        }); 
	        thumbSmall.removeClass('active');
	        $(this).addClass('active');	        
	      });
	    }).eq(0).addClass('active');

	    // declare the thumbwrapper
	    var thumbWrapper = $('.cloud-thumbwrapper');
      if (option.hoverZoom == 1) {
        thumbWrapper.width(option.previewWidth).height(option.previewHeight).imageEnlarge({
          multiple: option.multiple,
          mouseInSpeed : option.mouseOutSpeed,
          mouseOutSpeed : option.mouseInSpeed,
          zindex : option.zindex,
          topOffset : option.topOffset,
          leftOffset : option.leftOffset
        });
      }
    }
  } // end of cloud zoom plus thumbnail
	
	// integrate colorbox if requested
	if (option.colorbox == 1) {
    $('a.cloud-zoom').colorbox(option);
    
    // emulate click if both colorbox and cloud zoom enabeld
    if (option.colorbox == 1 && option.zoom == 1) {
      $('.cloud-zoom-lens').live('click', function() {
          $(this).parent('a').click();
      });
      $('.mousetrap').live('click', function() {
        $(this).parent().find('a.cloud-zoom').click();
      });
    }
	}
});