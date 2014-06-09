<?php

/**
 * @file
 * Defines the inline entity form controller for Nodes.
 */

class AssetInlineEntityFormController extends EntityInlineEntityFormController {

  /**
   * Overrides EntityInlineEntityFormController::defaultLabels().
   */
  public function defaultLabels() {
    $labels = array(
      'singular' => t('asset'),
      'plural' => t('assets'),
    );
    return $labels;
  }

  /**
   * Overrides EntityInlineEntityFormController::defaultSettings().
   */
  public function defaultSettings() {
    $defaults = parent::defaultSettings();

    // Allow ref. existing
    $defaults['allow_existing'] = TRUE;

    return $defaults;
  }

  /**
   * Overrides EntityInlineEntityFormController::entityForm().
   */
  public function entityForm($entity_form, &$form_state) {
    $asset = $entity_form['#entity'];
    //$type = node_type_get_type($asset);
    //$extra_fields = field_info_extra_fields('node', $asset->type, 'form');

    // Do some prep work on the node, similarly to node_form().
    //if (!isset($asset->title)) {
    //  $asset->title = NULL;
    //}
    //node_object_prepare($asset);

    $entity_form['title'] = array(
      '#type' => 'textfield',
      //'#title' => check_plain($type->title_label),
      '#title' => t('Title'),
      '#required' => TRUE,
      //'#default_value' => $asset->title,
      '#maxlength' => 255,
      // The label might be missing if the Title module has replaced it.
      //'#weight' => !empty($extra_fields['title']) ? $extra_fields['title']['weight'] : -5,
    );

    $langcode = entity_language('asset', $asset);
    field_attach_form('asset', $asset, $entity_form, $form_state, $langcode);

    return $entity_form;
  }

  /**
   * Overrides EntityInlineEntityFormController::entityFormSubmit().
   */
  public function entityFormSubmit(&$entity_form, &$form_state) {
    parent::entityFormSubmit($entity_form, $form_state);

    asset_submit($entity_form['#entity']);
    $child_form_state = form_state_defaults();
    $child_form_state['values'] = drupal_array_get_nested_value($form_state['values'], $entity_form['#parents']);
    foreach (module_implements('asset_submit') as $module) {
      $function = $module . '_asset_submit';
      $function($entity_form['#entity'], $entity_form, $child_form_state);
    }
  }
}
