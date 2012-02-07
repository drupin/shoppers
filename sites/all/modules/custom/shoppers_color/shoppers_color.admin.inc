<?php

/**
 * @file
 * Provides the administration page for Shoppers Color.
 */

/**
 * Administration settings page
 */
function shoppers_color_admin() {
  $form['shoppers_color_buttons'] = array(
      '#type' => 'radios',
      '#title' => t('Buttons Color'),
      '#default_value' => variable_get('shoppers_color_buttons', 'red'),
      '#options' => array('red' => t('red'), 'green' => t('green'), 'blue' => t('blue')),
      '#description' => t('Set the color of Add to cart buttons etc.'),
  );
  return system_settings_form($form);
}