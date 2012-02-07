var $QSC; // QuicksandContainer
(function ($) {
  Drupal.behaviors.views_quicksand =  {
    attach: function(context) {
      if(Drupal.settings.views_quicksand){
        $.each(Drupal.settings.views_quicksand, function(id) {
          var config = this;

          // build the view-selector from the view-settings 
          var displaySelector = '.view-id-' + config.viewname +
          '.view-display-id-' + config.display;
          // prepare JQuery objects for later use
          var $viewContainer = $(displaySelector);
          var $viewContent = $viewContainer.find('.view-content');
          var $viewPager = $viewContainer.find('.item-list').has('.pager');
          
          // if the view has no results there is also no view-content
          // this prevents the animation to run
          // therefore we create a dummy container
          if ($viewContent.length == 0) {
            $viewContent = $('<div></div>"');
            $viewContainer.append($viewContent)
          }

          if ($QSC == null) {
            // if QSC is empty the page has been loaded normally
            // and we have to create the container
            $viewContainer.after('<div class="views-quicksand-container"></div>');
            $QSC = $('.views-quicksand-container');
            $QSC.html($viewContent.html());
            $viewContent.hide();
            // move pager
            $viewPager.hide();
            movePager($viewPager);
          }
          else {
            // otherwise a ajax view request has happened and we have to
            // fire the quicksand animation
            $viewContent.once('views-quicksand', function() {
              $(this).hide();
              // hide the new pager and remove the old one
              $viewPager.hide();
              $QSC.next().remove();
              // define custom events
              var before = jQuery.Event('beforeQuicksandAnimation');
              before.callback = undefined;
              $QSC.trigger(before);
              var after = jQuery.Event('afterQuicksandAnimation');
              after.callback = undefined;
              $QSC.trigger(after);
              // do the animation
              $QSC.quicksand($(this).find('.views-row'),
              {
                adjustHeight: config.adjustHeight,
                attribute: function(v) {
                  return $(v).find(config.element).attr(config.attribute);
                },
                duration: parseInt(config.duration, 10),
                easing: config.easing,
                useScaling: true,
                enhancement: before.callback
              },
              after.callback);
              // move pager to views-quicksand-container
              movePager($viewPager);
            });
          }
        });
      }
    }
  };

  function movePager($viewPager){
    // clone the pager and put behind the QSC
    $viewPagerClone = $viewPager.clone(true);
    $QSC.after($viewPagerClone.show());
    // as clicking the pager only works if it is inside the view
    // container, we forward the click of the cloned one to the original
    $('a',$viewPagerClone).click(function(el){
      el.preventDefault();
      // the delegation could be done more elegantly with jQuery 1.6
      // $viewPagerClone.find(el.target).click()
      $('a[href=' + $(el.target).attr('href') + ']', $viewPager).click();
    });
  }
})(jQuery);
