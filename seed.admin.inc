<?php

/**
 * Taxonomy display settings form.
 */
function seed_taxonomy_display_form($form_state, $type) {
  $form = array(
    '#tree' => TRUE,
    '#theme' => 'seed_taxonomy_display_form',
  );
  $settings = variable_get("seed_taxonomy_display_{$type}", array());

  $vocabs = taxonomy_get_vocabularies();
  foreach ($vocabs as $vid => $info) {
    if (empty($info->nodes[$type])) {
      unset($vocabs[$vid]);
    }
  }

  $options = array(
    0 => t('Default'),
    SEED_TAXONOMY_FIELDTERMS => t('Field style: comma separated'),
    SEED_TAXONOMY_FIELDLIST => t('Field style: unordered list'),
  );

  if (!empty($vocabs)) {
    foreach ($vocabs as $vid => $info) {
      $form[$vid] = array(
        '#tree' => TRUE,
        '#title' => $info->name,
      );
      $form[$vid]['label'] = array(
        '#size' => 30,
        '#type' => 'textfield',
        '#default_value' => !empty($settings[$vid]['label']) ? $settings[$vid]['label'] : '',
      );
      $form[$vid]['teaser'] = array(
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => !empty($settings[$vid]['teaser']) ? $settings[$vid]['teaser'] : 0,
      );
      $form[$vid]['page'] = array(
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => !empty($settings[$vid]['page']) ? $settings[$vid]['page'] : 0,
      );
      $form[$vid]['weight'] = array(
        '#type' => 'weight',
        '#default_value' => !empty($settings[$vid]['weight']) ? $settings[$vid]['weight'] : 0,
      );
    }
    $form = array("seed_taxonomy_display_{$type}" => $form);
    $form = system_settings_form($form);
  }
  else {
    $form['message'] = array(
      '#type' => 'markup',
      '#value' => t('There are no vocabularies enabled for this content type.'),
    );
  }
  return $form;
}

/**
 * Theme the taxonomy display settings form.
 */
function theme_seed_taxonomy_display_form($form) {
  $output = '';

  $header = array(t('Vocabulary'), t('Custom label'), t('Teaser'), t('Page view'), t('Weight'));
  $rows = array();
  foreach (element_children($form) as $vid) {
    $rows[] = array(
      $form[$vid]['#title'],
      drupal_render($form[$vid]['label']),
      drupal_render($form[$vid]['teaser']),
      drupal_render($form[$vid]['page']),
      drupal_render($form[$vid]['weight']),
    );
  }

  $output .= theme('table', $header, $rows);
  $output .= drupal_render($form);

  return $output;
}

/**
 * Seed settings form.
 */
function seed_settings_form_basic(&$form_state) {
  $form = array();
  $seed_forms = variable_get('seed_forms', array());
  $seed_attachments = variable_get('seed_attachments', array());
  $seed_book = variable_get('seed_book', array());

  $form['seed_forms'] = array(
    '#type' => 'fieldset',
    '#title' => t('Form settings'),
    '#tree' => TRUE,
  );
  $form['seed_forms']['numbered'] = array(
    '#type' => 'select',
    '#title' => t('Item display'),
    '#options' => array(FALSE => t('Default'), TRUE => t('Numbered')),
    '#default_value' => isset($seed_forms['numbered']) ? $seed_forms['numbered'] : FALSE,
  );
  if (module_exists('thickbox') || module_exists('mp3player')) {
    $form['seed_attachments'] = array(
      '#type' => 'fieldset',
      '#title' => t('Node attachment settings'),
      '#tree' => TRUE,
    );
    if (module_exists('thickbox')) {
      $form['seed_attachments']['imagehandling'] = array(
        '#type' => 'select',
        '#title' => t('Image Attachments Display'),
        '#options' => array(FALSE => t('Default'), TRUE => t('Use ThickBox')),
        '#default_value' => isset($seed_attachments['imagehandling']) ? $seed_attachments['imagehandling'] : FALSE,
      );
      $options = imagecache_presets();
      foreach($options as $k=>$v) {
        $options[$k] = $k;
      }
      $form['seed_attachments']['imagecache_setting'] = array(
        '#type' => 'select',
        '#title' => t('ImageCache Setting'),
        '#options' => $options,
        '#default_value' => isset($seed_attachments['imagecache_setting']) ? $seed_attachments['imagecache_setting'] : FALSE,
      );
      $form['seed_attachments']['items_per_row'] = array(
        '#type' => 'select',
        '#title' => t('Images Per Row'),
        '#options' => array(1 => 1, 2 => 2, 3 => 3, 4 => 4, 5 => 5, 6 => 6, 7 => 7, 8 => 8, 9 => 9),
        '#default_value' => isset($seed_attachments['items_per_row']) ? $seed_attachments['items_per_row'] : 3,
      );
    }
    if (module_exists('mp3player')) {
      $form['seed_attachments']['audiohandling'] = array(
        '#type' => 'select',
        '#title' => t('Audio Attachments Display'),
        '#options' => array(FALSE => t('Default'), TRUE => t('Use Audio Player')),
        '#default_value' => isset($seed_attachments['audohandling']) ? $seed_attachments['audohandling'] : FALSE,
      );
    }
  }
  if (module_exists('book')) {
    $form['seed_book'] = array(
      '#type' => 'fieldset',
      '#title' => t('Book settings'),
      '#tree' => TRUE,
    );
    $form['seed_book']['book_nav'] = array(
      '#type' => 'select',
      '#title' => t('Display book navigation under node content'),
      '#options' => array(TRUE => t('Yes'), FALSE => t('No')),
      '#default_value' => isset($seed_book['book_nav']) ? $seed_book['book_nav'] : FALSE,
    );
  }
  return system_settings_form($form);
}
