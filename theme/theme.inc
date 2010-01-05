<?php

/**
 * Helper function to number a form tree intelligently.
 */
function seed_number_form($form) {
  // Element numbering loop
  $output = '';
  foreach (element_children($form) as $element) {
    if (seed_is_empty($form[$element])) {
      $output .= drupal_render($form[$element]);
    }
    else {
      $output .= theme('seed_numbered_element', drupal_render($form[$element]));
    }
  }
  return $output;
}

/**
 * Helper function to recurse down a FAPI element and determine whether
 * it has any visible elements to the user.
 */
function seed_is_empty($form) {
  $empty = TRUE;
  if (empty($form['#type'])) {
    foreach (element_children($form) as $e) {
      $empty = $empty && seed_is_empty($form[$e]);
    }
  }
  else if (!in_array($form['#type'], array('value', 'hidden', 'token'))) {
    $empty = FALSE;
  }
  return $empty;
}

/**
 * Theme function to add numbering markup to form elements.
 */
function theme_seed_numbered_element($markup) {
  drupal_add_css(drupal_get_path('module', 'seed') .'/theme/seed.css');
  static $counter;
  $output = '';
  if (!empty($markup)) {
    if (!isset($counter)) {
      $counter = 1;
    }
    $output .= "<div class='form-item-counter'><span class='counter'>$counter</span>$markup</div>";
    $counter++;
  }
  return $output;
}

/**
 * Theme a filter form element
 */
function theme_seed_filter_form($form) {
  drupal_add_css(drupal_get_path('module', 'seed') .'/theme/seed.css');
  unset($form['#title']);
  if (!empty($form['#options'])) {
    $select = theme('select', $form);
  }
  $help = theme('filter_tips_more_info');
  $output = "<div class='filter-options clear-block'>{$select} {$help}</div>";
  return $output;
}

/**
 * Override of theme_filter_tips_more_info().
 */
function seed_filter_tips_more_info() {
  return '<div class="filter-help">'. l(t('Formatting help'), 'filter/tips', array('attributes' => array('target' => '_blank'))) .'</div>';
}

/**
 * Theme a links array into a comma-separated term list.
 */
function theme_seed_term_list($links = array()) {
  $list = array();
  foreach ($links as $link) {
    $list[] = l($link['title'], $link['href'], $link);
  }
  return implode(', ', $list);
}

/**
 * Generate a nice looking byline
 */
function theme_seed_byline($object) {
  static $accounts;
  $output = '';

  // This is a comment
  if (isset($object->cid)) {
    $date = $object->timestamp;
    $uid = !empty($object->uid) ? $object->uid : NULL;
    $name = !empty($object->name) ? $object->name : NULL;
  }
  // This is a node
  else if ($object->nid) {
    $date = $object->created;
    $uid = !empty($object->uid) ? $object->uid : NULL;
    $name = !empty($object->name) ? $object->name : NULL;
  }

  // Build output
  $date = seed_rel_date($date);
  if ($uid) {
    if (!isset($accounts[$uid])) {
      $accounts[$uid] = user_load(array('uid' => $uid));
    }
    if (!empty($accounts[$uid])) {
      $picture = theme('user_picture', $accounts[$uid]);
      $username = theme('username', $accounts[$uid]);
    }
  }
  if (empty($username)) {
    $username = theme('username', $object);
  }
  return "$picture $username <span class='date'>$date</span>";
}

/**
 * Use mod math to generate an integer for crayon coloring.
 */
function theme_seed_crayon($id) {
  drupal_add_css(drupal_get_path('module', 'seed') .'/theme/seed.css');
  static $crayon;
  static $strings;

  // Initialize static vars.
  if (!isset($crayon)) {
    $crayon = array();
    $strings = array();
  }

  // Do a string to int conversion if necessary
  if (!is_numeric($id)) {
    if (!isset($strings[$id])) {
      // @TODO: Determine the best distinct string to distinct int conversion here.
      $strings[$id] = abs(crc32($id));
    }
    $id = $strings[$id];
  }

  // Return the modulo crayon color id
  if (isset($crayon[$id])) {
    return $crayon[$id];
  }
  else {
    $modulo = $id % 16;

    // Try 5 times to avoid color collision
    $try = 0;
    while (in_array($modulo, $crayon, TRUE) && $try < 5) {
      $modulo = ($modulo + 1) % 16;
      $try++;
    }

    $crayon[$id] = $modulo;
    return $crayon[$id];
  }
}

/**
 * A generalized hover popup style.
 */
function theme_seed_crayon_popup($id, $label, $color = NULL) {
  $style = $output = '';

  $crayon = !empty($id) ? theme('seed_crayon', $id) : NULL;
  if (!empty($color)) {
    $style = (strpos($color, '#') !== FALSE) ? "background-color: {$color}" : "background-color: #{$color}";
  }
  $content = empty($content) ? $label : $content;

  $output .= "<span class='crayon-popup'>";
  $output .= "<span class='crayon-popup-label'>";
  if (isset($crayon)) {
    $output .= "<span class='crayon-marker crayon-{$crayon}' style='{$style}'></span>";
  }
  $output .= $label;
  $output .= "</span>";
  $output .= "</span>";
  return $output;
}

/**
 * A small swatch of color with label that has a popup with more info on hover.
 */
function theme_seed_crayon_swatch($id, $label, $popup = NULL, $path = NULL, $options = array(), $color = NULL) {
  $popup = empty($popup) ? $label : $popup;
  $popup = theme('seed_crayon_popup', $id, $popup, $color);

  $crayon = theme('seed_crayon', $id);
  if (!empty($color)) {
    $style = (strpos($color, '#') !== FALSE) ? "background-color: {$color}" : "background-color: #{$color}";
  }
  $options = array(
    'html' => true,
    'attributes' => array(
      'class' => 'crayon crayon-swatch crayon-'. $crayon,
      'style' => !empty($style) ? $style : NULL,
    ),
  ) + $options;

  $content = "<span class='label'>$label</span>$popup";

  if (!empty($path)) {
    return l($content, $path, $options);
  }
  else {
    $attr = drupal_attributes($options['attributes']);
    return "<span $attr>$content</span>";
  }
}

/**
 *  Attachments
 */
function seed_upload_attachments($files) {
  drupal_add_css(drupal_get_path('module', 'seed') .'/theme/seed.css');
  $output = '';
  $header = array(t('Preview'), t('Attachment'), t('Size'));
  $rows = $cells = array();
  foreach ($files as $file) {
    if ($file->list) {
      $href = check_url(($file->fid ? file_create_url($file->filepath) : url(file_create_filename($file->filename, file_create_path()))));
      $text = $file->description ? $file->description : $file->filename;
      $fileview = theme('seed_fileview', $file);
      $rows[] = array(
        array('data' => $fileview, 'class' => 'preview'),
        array('data' => l($text, $href), 'class' => 'attachment'),
        array('data' => format_size($file->filesize), 'class' => 'filesize'),
      );
    }
  }
  if (count($rows)) {
    $output .= theme('table', $header, $rows, array('id' => 'attachments'));
  }
  return $output;
}

/**
 * Image Attachments use Thickbox.
 */
function theme_seed_thickbox($images) {
  $seed_attachments = variable_get('seed_attachments', array());
  $imagecache = $seed_attachments['imagecache_setting'];
  $items_per_row = $seed_attachments['items_per_row'];
  foreach($images as $v) {
    $image = theme('imagecache', $imagecache, $v->filepath, $v->filename, $v->description, array('class' =>'thickbox-image'));
    $items[] = array(
      'data' => l($image, $v->filepath, array('attributes' => array('class' =>'thickbox'), 'html' => TRUE)),
      );
    }
  $header = NULL;
  $rows = array_chunk($items, $items_per_row, TRUE);
  $output .= theme('table', $header, $rows);
  return $output;
}

/**
 * Audio Attachments use Audio Player
 */
function theme_seed_audio($audio) {
  $output = '';
  foreach ($audio as $k => $v) {
    $output .= theme('mp3player', 'Default', url($v->filepath)); 
  }
  return $output;
}

/**
 * Backwards-compatibility wrapper to calls to "seed_fileview".
 */
function seed_fileview($file) {
  return theme('seed_fileview');
}

/**
 * Show icons or a preview of image filetypes
 */
function theme_seed_fileview($file) {
  drupal_add_css(drupal_get_path('module', 'seed') .'/theme/seed.css');
  $o = '';
  $href = check_url(($file->fid ? file_create_url($file->filepath) : url(file_create_filename($file->filename, file_create_path()))));
  switch($file->filemime) {
    case 'image/jpeg':
    case 'image/gif':
    case 'image/png':
      if (module_exists('imagecache')) {
        $preset = variable_get('seed_imagecache_attachment', 'attachment');
        $imagecache_path =  imagecache_create_url($preset, $file->filepath);
        $o = l('', $href, array('attributes' => array('style' => "background-image: url('$imagecache_path')")));
      }
      break;
    default:
      $o = l('', $href, array('attributes' => array('class' => 'icon ' . seed_id_safe($file->filemime))));
    break;
  }
  return "<div class='fileview'>$o</div>";
}
