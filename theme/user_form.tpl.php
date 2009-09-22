<div class='form clear-block <?php if (!empty($form_classes)) print $form_classes ?>'>

<?php if (!empty($sidebar)): ?>
  <div class='sidebar'><?php print drupal_render($sidebar) ?></div>
<?php endif; ?>

<div class='main'><?php print drupal_render($form) ?></div>

<?php if ($buttons): ?>
  <div class='buttons clear-block'><?php print drupal_render($buttons) ?></div>
<?php endif; ?>

</div>
