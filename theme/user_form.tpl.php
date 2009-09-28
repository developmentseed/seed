<div class='form clear-block <?php print $form_classes ?>'>

<div class='main'><?php print drupal_render($form) ?></div>

<?php if (!empty($sidebar)): ?>
  <div class='sidebar'><?php print drupal_render($sidebar) ?></div>
<?php endif; ?>

<?php if ($buttons): ?>
  <div class='buttons clear-block'><?php print drupal_render($buttons) ?></div>
<?php endif; ?>

</div>
