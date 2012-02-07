<?php

/**
 * @file
 * Provides the administration page for Equal Heights.
 */

/**
 * Administration settings page
 */
function equalheights_admin() {
  $form = array();
  $description = t('Add list of of CSS classes that you want to be of equal height ');
  $description .= t(' each on a new line (one class per set of elements). ');
  $description .= t('Optionally you can specify minimum and maximum height in pixels after a colon, separated by the comma ');
  $description .= t('(the format is \'classname:minheight,maxheight\'. ');
  $description .= t('If you use only one number it will be treated as a minimum height. ');
  $description .= t('For example, \'.equalheight:100,500\' will set the height of all elements with the class \'equalheight\' ');
  $description .= t('to at least 100px but no more than 500px. ');
  $description .= t('(<a href="@about">Read more about the plugin.</a>) ', array('@about' => 'http://www.cssnewbie.com/equalheights-jquery-plugin/'));
  $form['equalheights_css_classes'] = array(
    '#type' => 'textarea',
    '#title' => t('CSS Classes'),
    '#default_value' => variable_get('equalheights_css_classes', ''),
    '#description' => $description,
  );
  $form['equalheights_overflow'] = array(
      '#type' => 'radios',
      '#title' => t('Overflow property'),
      '#default_value' => variable_get('equalheights_overflow', 'visible'),
      '#options' => array('auto' => t('auto'), 'visible' => t('visible'), 'hidden' => t('hidden')),
      '#description' => t('Set overflow value for the selected elements'),
  );
  return system_settings_form($form);
} // function equalheights_admin