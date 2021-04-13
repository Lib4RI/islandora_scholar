<?php

/**
 * @file
 * Administration form.
 */

/**
 * Defines the admin settings form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form definition.
 */
function bibutils_admin_form(array $form, array &$form_state) {

  $dateAry = array(
    '0' => "Do not export any Conference Date ('DA')",
    '1' => 'Try RIS-comliance, but tolerate period notation e.g. "April 17-23th, 2019"',
    '2' => 'Try RIS-comliance by reducing a period to starting date e.g. "2019/04/17"',
    '3' => 'RIS-compliant notation of a period e.g. "2019/04/17/ until /2019/04/23"',
  );
  $form['islandora_scholar_ris_date_strict'] = array(
    '#prefix' => '<hr><br><b>RIS related:</b><ul>',
    '#type' => 'select',
    '#title' => t("RIS Export: Handing of Conference Date/Period"),
    '#description' => t( 'Select how the Conference Date should be exported, if at all.' ), 
    '#options' => $dateAry,
    '#default_value' => variable_get('islandora_scholar_ris_date_strict','0'),
    '#multiple' => FALSE,
    '#required' => FALSE,
  );

  $form['bibutils_ris2xml_path'] = array(
    '#type' => 'textfield',
    '#title' => t('RIS to MODS executable'),
    '#description' => t('The path to ris2xml.'),
    '#default_value' => variable_get('bibutils_ris2xml_path', 'ris2xml'),
    '#suffix' => '</ul><br><hr><br>',
  );

  $form['bibutils_endx2xml_path'] = array(
    '#prefix' => '<b>Endnote related:</b><ul>',
    '#type' => 'textfield',
    '#title' => t('Endnote XML to MODS executable'),
    '#description' => t('The path to endx2xml.'),
    '#default_value' => variable_get('bibutils_endx2xml_path', 'endx2xml'),
    '#suffix' => '</ul><br><hr><br>',
  );

  return system_settings_form($form);
}
