<?php

/**
 * @file
 * Provides the administration page for Personalize module.
 */

/**
 * Administration settings page
 */
function personalize_admin() {
	
	$sel = variable_get('personalize_theme', 'classic');
	$opts = array(
			'classic' => t('Classic'),
			'modern' => t('Modern'),
			'light-blue' => t('Light blue')
		);
		
	foreach ($opts as $opt => $val) {
		$style[$opt] = ($sel != $opt) ? 'style=display:none' : '';
	}
	
	$form['theme_preset'] = array(
		'#type' => 'select',
		'#title' => t('Choose your theme'),
		'#default_value' => $sel,
		'#options' => $opts,
		'#field_suffix' => '<div class="theme_preset_info">
			<div id="classic-theme" ' . $style['classic'] . '>Our default choise! A simple but classic skin bla bla</div>
			<div id="modern-theme" ' . $style['modern'] . '>This elegant theme combines rounded lines with bla bla</div>
			<div id="light-blue-theme" ' . $style['light-blue'] . '>This theme has light colors bla bla</div>
		</div>',
		'#description' => t('Choose one of the available themes to instantly change the look of your e-shop.'),
	);
  
	$form['#attached']['js'] = array(
		drupal_get_path('module', 'personalize') . '/js/form.js',
	);

  return system_settings_form($form);
}