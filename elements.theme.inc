<?php

/**
 * @file
 * The theme include file for the elements module.
 *
 * Contains the theme functions for all the elements module elements.
 */

/**
 * Returns HTML for a table with radio buttons or checkboxes.
 *
 * An example of per-row options:
 * @code
 * $options = array();
 * $options[0]['title'] = "A red row"
 * $options[0]['#attributes'] = array ('class' => array('red-row'));
 * $options[1]['title'] = "A blue row"
 * $options[1]['#attributes'] = array ('class' => array('blue-row'));
 *
 * $form['myselector'] = array (
 * '#type' => 'tableselect',
 * '#title' => 'My Selector'
 * '#options' => $options,
 * );
 * @endcode
 *
 * @param $element
 *   An associative array containing the properties and children of the
 *   tableselect element. Properties used: #header, #options, #empty, and
 *   #js_select. The #options property is an array of selection options; each
 *   array element of #options is an array of properties. These properties can
 *   include #attributes, which is added to the table row's HTML attributes;
 *   see theme_table().
 *
 * @ingroup themeable
 */
function theme_tableselect($element) {
  $rows = array();
  $header = $element['#header'];
  if (!empty($element['#options'])) {
    // Generate a table row for each selectable item in #options.
    foreach (element_children($element) as $key) {
      $row = array();

      $row['data'] = array();
      if (isset($element['#options'][$key]['#attributes'])) {
        $row += $element['#options'][$key]['#attributes'];
      }

      // Render the checkbox / radio element.
      $element[$key]['#printed'] = NULL;  // Drupal 6 only
      $row['data'][] = drupal_render($element[$key]);

      // As theme_table only maps header and row columns by order, create the
      // correct order by iterating over the header fields.
      foreach ($element['#header'] as $fieldname => $title) {
        $row['data'][] = $element['#options'][$key][$fieldname];
      }
      $rows[] = $row;
    }
    // Add an empty header or a "Select all" checkbox to provide room for the
    // checkboxes/radios in the first table column.
    if ($element['#js_select']) {
      // Add a "Select all" checkbox.
      //drupal_add_js('misc/tableselect.js');
      //array_unshift($header, array('class' => 'select-all'));
      array_unshift($header, theme('table_select_header_cell')); // Drupal 6 only
    }
    else {
      // Add an empty header when radio buttons are displayed or a "Select all"
      // checkbox is not desired.
      array_unshift($header, '');
    }
  }

  // Add the 'empty' row message if available.
  if (!count($rows) && $element['#empty']) {
    $header_count = 0;
    foreach ($header as $header_cell) {
      if (is_array($header_cell)) {
        $header_count += isset($header_cell['colspan']) ? $header_cell['colspan'] : 1;
      }
      else {
        $header_count++;
      }
    }
    $rows[] = array(array(
      'data' => $element['#empty'],
      'colspan' => $header_count,
      'class' => 'empty message',
    ));
  }

  return theme('table', $header, $rows, $element['#attributes']);
}

/**
 * Helper function to format an HTML 5 form element.
 *
 * @param $element
 *   An associative array containing the properties of the element.
 *
 * @return
 *   A themed HTML string representing the form field.
 */
function theme_element_html5($element) {
  $element['#attributes'] = array_merge(array(
    'type' => substr($element['#type'], 0, -5),
    'name' => $element['#name'],
    'id' => $element['#id'],
    'value' => $element['#value'],
  ), $element['#attributes']);

  if (!empty($element['#size'])) {
    $element['#attributes'] += array('size' => $element['#size']);
  }
  if (!empty($element['#maxlength'])) {
    $element['#attributes'] += array('maxlength' => $element['#maxlength']);
  }
  if (!empty($element['#placeholder'])) {
    $element['#attributes'] += array('placeholder' => $element['#placeholder']);
  }

  $class = array('form-' . $element['#type']);
  $extra = '';
  $output = '';

  if ($element['#autocomplete_path'] && menu_valid_path(array('link_path' => $element['#autocomplete_path']))) {
    drupal_add_js('misc/autocomplete.js');
    $class[] = 'form-autocomplete';
    $extra =  '<input class="autocomplete" type="hidden" id="'. $element['#id'] .'-autocomplete" value="'. check_url(url($element['#autocomplete_path'], array('absolute' => TRUE))) .'" disabled="disabled" />';
  }
  _form_set_class($element, $class);

  if (isset($element['#field_prefix'])) {
    $output .= '<span class="field-prefix">'. $element['#field_prefix'] .'</span> ';
  }

  $output .= '<input' . drupal_attributes($element['#attributes']) .' />';

  if (isset($element['#field_suffix'])) {
    $output .= ' <span class="field-suffix">'. $element['#field_suffix'] .'</span>';
  }

  return theme('form_element', $element, $output) . $extra;
}

/**
 * Format an e-mail form field.
 *
 * @param $element
 *   An associative array containing the properties of the element.
 *
 * @return
 *   A themed HTML string representing the form field.
 */
function theme_emailfield($element) {
  return theme_element_html5($element);
}

/**
 * Format a search form field.
 *
 * @param $element
 *   An associative array containing the properties of the element.
 *
 * @return
 *   A themed HTML string representing the form field.
 */
function theme_searchfield($element) {
  return theme_element_html5($element);
}

/**
 * Format a mail form field.
 *
 * @param $element
 *   An associative array containing the properties of the element.
 *
 * @return
 *   A themed HTML string representing the form field.
 */
function theme_telfield($element) {
  return theme_element_html5($element);
}

/**
 * Format an URL form field.
 *
 * @param $element
 *   An associative array containing the properties of the element.
 *
 * @return
 *   A themed HTML string representing the form field.
 */
function theme_urlfield($element) {
  return theme_element_html5($element);
}

/**
 * Format a number form field.
 *
 * @param $element
 *   An associative array containing the properties of the element.
 *
 * @return
 *   A themed HTML string representing the form field.
 */
function theme_numberfield($element) {
  if (isset($element['#min'])) {
    $element['#attributes']['min'] = $element['#min'];
  }
  if (isset($element['#max'])) {
    $element['#attributes']['max'] = $element['#max'];
  }
  if (isset($element['#step'])) {
    $element['#attributes']['step'] = $element['#step'];
  }

  return theme_element_html5($element);
}

/**
 * Format a range form field.
 *
 * @param $element
 *   An associative array containing the properties of the element.
 *
 * @return
 *   A themed HTML string representing the form field.
 */
function theme_rangefield($element) {
  return theme_numberfield($element);
}
