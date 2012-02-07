<?php
/**
 * @file
 * Functions needed for devel_generate integration.
 */

/**
 * Implements hook_devel_generate().
 */
function double_field_devel_generate($object, $field, $instance, $bundle) {
  return array(
    'first' => user_password($field['settings']['first_maxlength']),
    'second' => user_password($field['settings']['second_maxlength']),
  );
}
