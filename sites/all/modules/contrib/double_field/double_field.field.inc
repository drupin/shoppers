<?php

/**
 * @file
 *
 */
 
/**
 * Implements hook_field_info().
 */
function double_field_field_info() {
  return array(
    'double_field' => array(
      'label' => t('Double field'),
      'description' => t('Double text field'),
      'default_widget' => 'double_field',
      'default_formatter' => 'double_field_inline',
      'settings' => array(
        'first' => array('maxlength' => 255),
        'second' => array('maxlength' => 255),
      ),
      // Integrate with the Entity Metadata module.
      'property_type' => 'double_field',
      'property_callbacks' => array('double_field_property_callback'),
    ),
  );
}

/**
 * Implements hook_field_settings_form().
 */
function double_field_field_settings_form($field, $instance, $has_data) {
  $settings = $field['settings'];
  $form['first']['maxlength'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum length of the first textfield'),
    '#default_value' => $settings['first']['maxlength'],
    '#required' => TRUE,
    '#description' => t('The maximum length of the second textfield in characters.'),
    '#element_validate' => array('_element_validate_integer_positive'),
    '#disabled' => $has_data,
  );
  $form['second']['maxlength'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum length of the second textfield'),
    '#default_value' => $settings['second']['maxlength'],
    '#required' => TRUE,
    '#description' => t('The maximum length of the second field in characters.'),
    '#element_validate' => array('_element_validate_integer_positive'),
    '#disabled' => $has_data,
  );
  return $form;
}

/**
 * Implements hook_field_widget_info().
 *
 * widget settings
 */
function double_field_field_widget_info() {
  $settings = array(
    'general' => array(
      'required' => TRUE,
      'prefix' => '',
      'suffix' => '',
    ),
  );
  $subwidgets = array(
    'textfield' => t('Text field'),
    'checkbox' => t('Checkbox'),
    'select' => t('Select list'),
    'textarea' => t('Textarea'),
  );
  foreach ($subwidgets as $first_subwidget => $first_subwidget_label) {
    foreach ($subwidgets as $second_subwidget => $second_subwidget_label) {
      $widgets[$first_subwidget . '_&_' . $second_subwidget] = array(
        'label' => $first_subwidget_label . ' & ' . $second_subwidget_label,
        'field types' => array('double_field'),
        'settings' => array(
          'inline' => TRUE,
          'first' => $settings,
          'second' => $settings,
        ),
      );
    }
  }
  return $widgets;
}

/**
 * Implements hook_field_widget_settings_form().
 */
function double_field_field_widget_settings_form($field, $instance) {

  $settings = $instance['widget']['settings'];

  $form['inline'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display as inline element'),
    '#default_value' => $settings['inline'],
  );
  $subfield_types = explode('_&_', $instance['widget']['type']);
  foreach (array('first', 'second') as $key => $subfield) {
    $form[$subfield] = array(
      '#title' => t('@subfield subfield – @subwidget', array('@subfield' => $subfield, '@subwidget' => $subfield_types[$key])),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form[$subfield]['general'] = array(
      '#title' => t('Common settings'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form[$subfield]['general']['required'] = array(
      '#type' => 'checkbox',
      '#title' => t('Required'),
      '#default_value' => $settings[$subfield]['general']['required'],
      '#states' => array(
        'visible' => array(
          ':input[name="instance[required]"]' => array('checked' => TRUE),
        ),
      ),
    );
    $form[$subfield]['general']['prefix'] = array(
      '#type' => 'textfield',
      '#title' => t('Prefix'),
      '#maxlength' => 50,
      '#default_value' => $settings[$subfield]['general']['prefix'],
    );
    $form[$subfield]['general']['suffix'] = array(
      '#type' => 'textfield',
      '#title' => t('Suffix'),
      '#maxlength' => 50,
      '#default_value' => $settings[$subfield]['general']['suffix'],
    );

    $settings[$subfield] += double_field_widget_default_settings($subfield_types[$key]);
    switch ($subfield_types[$key]) {
      case 'textfield':
        $form[$subfield]['size'] = array(
          '#type' => 'textfield',
          '#title' => t('Size'),
          '#default_value' => $settings[$subfield]['size'],
          '#element_validate' => array('_element_validate_integer_positive'),
          '#size' => 3,
        );
        break;
      case 'checkbox':
        $form[$subfield]['title'] = array(
          '#type' => 'textfield',
          '#title' => t('Title'),
          '#default_value' => $settings[$subfield]['title'],
        );
        break;
      case 'select':
        $form[$subfield]['allowed_values'] = array(
          '#type' => 'textarea',
          '#subfield' => $subfield,
          '#title' => t('Allowed values list'),
          '#default_value' => list_allowed_values_string(empty($settings[$subfield]['allowed_values']) ? array() : $settings[$subfield]['allowed_values']),
          '#element_validate' => array('double_field_allowed_values_setting_validate'),
        );
        $form[$subfield]['allowed_values']['#description'] = '<p>' . t('The possible values this field can contain. Enter one value per line, in the format key|label.');
        $form[$subfield]['allowed_values']['#description'] .= '<br/>' . t('The key is the stored value. The label will be used in displayed values and edit forms.');
        $form[$subfield]['allowed_values']['#description'] .= '<br/>' . t('The label is optional: if a line contains a single string, it will be used as key and label.') . '</p>';
        break;
      case 'textarea':
        $form[$subfield]['cols'] = array(
          '#type' => 'textfield',
          '#title' => t('Columns'),
          '#default_value' => $settings[$subfield]['cols'],
          '#element_validate' => array('_element_validate_integer_positive'),
          '#size' => 3,
          '#description' => t('How many columns wide the textarea should be'),
        );
        $form[$subfield]['rows'] = array(
          '#type' => 'textfield',
          '#title' => t('Rows'),
          '#default_value' => $settings[$subfield]['rows'],
          '#element_validate' => array('_element_validate_integer_positive'),
          '#size' => 3,
          '#description' => t('How many rows high the textarea should be.'),
        );
        $form[$subfield]['resizable'] = array(
          '#type' => 'checkbox',
          '#title' => t('Resizable'),
          '#default_value' => $settings[$subfield]['resizable'],
          '#description' => t('Whether users should be allowed to resize the text area.'),
        );
        break;
    }
  }
  return $form;
}

/**
 * Element validate callback; check that the entered values are valid.
 */
function double_field_allowed_values_setting_validate($element, &$form_state) {
  $values = list_extract_allowed_values($element['#value'], 'list_text', FALSE);
  $maxlength = $form_state['values']['field']['settings'][$element['#subfield']]['maxlength'];

  // Bytes not characters.
  $options_length = array_map('strlen', array_keys($values));
  if ($options_length && max($options_length) > $maxlength) {
    form_error($element, t('Possible values of subfield cannot be greater maximum length of the subfield. '));
  }
  if (!is_array($values)) {
    form_error($element, t('Allowed values list: invalid input.'));
  }
  form_set_value($element, $values, $form_state);
}

/**
 * Implements hook_field_formatter_info().
 */
function double_field_field_formatter_info() {
  $settings = array(
    'first' => array(
      'prefix' => '',
      'suffix' => '&nbsp;',
    ),
    'second' => array(
      'prefix' => '',
      'suffix' => '',
    ),
  );
  return array(
    'double_field_inline' => array(
      'label' => t('Inline'),
      'field types' => array('double_field'),
      'settings' => $settings,
    ),
    'double_field_block' => array(
      'label' => t('Block'),
      'field types' => array('double_field'),
      'settings' => $settings,
    ),
    'double_field_fieldset' => array(
      'label' => t('Fieldset'),
      'field types' => array('double_field'),
      'settings' => $settings + array('collapsible' => TRUE, 'collapsed' => FALSE),
    ),
    'double_field_html_list' => array(
      'label' => t('HTML list'),
      'field types' => array('double_field'),
      'settings' => $settings + array('list_type' => 'ul'),
    ),
    'double_field_table' => array(
      'label' => t('Table'),
      'field types' => array('double_field'),
      'settings' => $settings + array(
        'table' => array(
          'number_column' => FALSE,
          'number_column_label' => '№',
          'first' => array('column_label' => ''),
          'second' => array('column_label' => ''),
        ),
      ),
    ),
    'double_field_accordion' => array(
      'label' => t('Accordion'),
      'field types' => array('double_field'),
      'settings' => $settings,
    ),
  );
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function double_field_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {

  $settings = $instance['display'][$view_mode]['settings'];
  $element = array('#tree' => TRUE);

  switch ($instance['display'][$view_mode]['type']) {
    case 'double_field_fieldset':
      $element['collapsible'] = array(
        '#type' => 'checkbox',
        '#title' => t('Collapsible'),
        '#default_value' => $settings['collapsible'],
      );
      $element['collapsed'] = array(
        '#type' => 'checkbox',
        '#title' => t('Collapsed'),
        '#default_value' => $settings['collapsed'],
      );
      break;

    case 'double_field_html_list':
      $element['list_type'] = array(
        '#type' => 'radios',
        '#title' => t('List type'),
        '#options' => array('ul' => t('Unordered list'), 'ol' => t('Ordered list')),
        '#default_value' => $settings['list_type'],
      );
      break;

    case 'double_field_table':
      $element['table'] = array(
        '#title' => t('Table'),
        '#type' => 'fieldset',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      $element['table']['number_column'] = array(
        '#type' => 'checkbox',
        '#title' => t('Enable row number column'),
        '#default_value' => $settings['table']['number_column'],
        '#attributes' => array('id' => 'number_column'),
      );
      $element['table']['number_column_label'] = array(
        '#type' => 'textfield',
        '#title' => t('Number column label'),
        '#size' => 30,
        '#maxlength' => 50,
        '#default_value' => $settings['table']['number_column_label'],
        '#states' => array(
          'visible' => array(
            '#number_column' => array('checked' => TRUE),
          ),
        ),
      );
      foreach (array('first', 'second') as $subfield) {
        $element['table'][$subfield]['column_label'] = array(
          '#type' => 'textfield',
          '#title' => t('@subfield column label', array('@subfield' => ucfirst($subfield))),
          '#size' => 30,
          '#maxlength' => 50,
          '#default_value' => $settings['table'][$subfield]['column_label'],
          '#tree' => TRUE,
        );
      }
      break;
  }

  // Commons settings.
  foreach (array('first', 'second') as $subfield) {
    $element[$subfield] = array(
      '#title' => t('@subfield subfield', array('@subfield' => $subfield)),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $element[$subfield]['prefix'] = array(
      '#type' => 'textfield',
      '#title' => t('Prefix'),
      '#size' => 30,
      '#maxlength' => 50,
      '#default_value' => $settings[$subfield]['prefix'],
    );
    $element[$subfield]['suffix'] = array(
      '#type' => 'textfield',
      '#title' => t('Suffix'),
      '#size' => 30,
      '#maxlength' => 50,
      '#default_value' => $settings[$subfield]['suffix'],
    );
  }
  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function double_field_field_formatter_settings_summary($field, $instance, $view_mode) {
  $settings = $instance['display'][$view_mode]['settings'];

  $summary = '';
  switch ($instance['display'][$view_mode]['type']) {
    case 'double_field_fieldset':
      $summary .= '<div>' . t('Collapsible: <em>@value</em>', array('@value' => $settings['collapsible'] ? 'Yes' : 'No')) . '</div>';
      $summary .= '<div>' . t('Collapsed: <em>@value</em>', array('@value' => $settings['collapsed'] ? 'Yes' : 'No')) . '</div>';
      break;
    case 'double_field_html_list':
      $summary .= '<div>' . t('List type: <em>@value</em>', array('@value' => $settings['list_type'] == 'ul' ? t('Unordered') : t('Ordered'))) . '</div>';
      break;
    case 'double_field_table':
      $summary .= '<div>' . t('Row number column: <em>@value</em>', array('@value' => $settings['table']['number_column'] ? 'Yes' : 'No')) . '</div>';
      $summary .= '<div>' . t('First column label: <em>@value</em>', array('@value' => $settings['table']['first']['column_label'])) . '</div>';
      $summary .= '<div>' . t('Second column label: <em>@value</em>', array('@value' => $settings['table']['second']['column_label'])) . '</div>';
      break;
  }
  $summary .= '<div>' . t('First prefix: <em>@value</em>', array('@value' => $settings['first']['prefix'])) . '</div>';
  $summary .= '<div>' . t('Second prefix: <em>@value</em>', array('@value' => $settings['second']['prefix'])) . '</div>';
  $summary .= '<div>' . t('First suffix: <em>@value</em>', array('@value' => $settings['first']['suffix'])) . '</div>';
  $summary .= '<div>' . t('Second suffix: <em>@value</em>', array('@value' => $settings['second']['suffix'])) . '</div>';

  return $summary;
}

/**
 * Alter the property info of date fields.
 */
function double_field_property_callback(&$info, $entity_type, $field, $instance, $field_type) {
  $property = &$info[$entity_type]['bundles'][$instance['bundle']]['properties'][$field['field_name']];
  $property['getter callback'] = 'entity_metadata_field_verbatim_get';
  $property['setter callback'] = 'entity_metadata_field_verbatim_set';
  $property['property info']['first'] = array(
    'type' => 'text',
    'label' => t('First subfield'),
    'setter callback' => 'entity_property_verbatim_set',
  );
  $property['property info']['second'] = array(
    'type' => 'text',
    'label' => t('Second subfield'),
    'setter callback' => 'entity_property_verbatim_set',
  );
  unset($property['query callback']);
}

