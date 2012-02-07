<?php

/**
 * @file
 * Checkout pane callback functions.
 */

/**
 * Settings form customizations for the pane.
 */
function commerce_extra_panes_contents_settings_form($checkout_pane) {
  $form = array();
  return $form;
}

/**
 * Checkout pane callback: returns the cart contents View for inclusion in the
 *   checkout form.
 */
function commerce_extra_panes_contents_checkout_form($form, &$form_state, $checkout_pane, $order) {
  $pane_form = array();
  $pane_info = explode('__', $checkout_pane['pane_id']);
  $node = current(entity_load($pane_info[1], array($pane_info[2])));
  $elements = node_view($node, 'checkout_pane');
  $pane_form[$checkout_pane['pane_id']]['node'] = array(
    '#markup' => theme('commerce_extra_panes_checkout_form', array('elements' => $elements)),
    '#weight' => 0,
  );
  return $pane_form;
}

/**
 * Checkout pane callback: returns the cart contents review data for the
 *   Review checkout pane.
 */
function commerce_extra_panes_contents_review($form, $form_state, $checkout_pane, $order) {
  $pane_info = explode('__', $checkout_pane['pane_id']);
  $node = reset(entity_load($pane_info[1], array($pane_info[2])));
  $node->rendered = node_view($node, 'checkout_pane');
  return theme('commerce_extra_panes_review', array('node' => $node));
}
