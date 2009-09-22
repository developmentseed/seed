<?php
// $Id: views-view-summary.tpl.php,v 1.5 2008/04/29 00:35:08 merlinofchaos Exp $
/**
 * @file views-view-summary.tpl.php
 * Default simple view template to display a list of summary lines
 *
 * @ingroup views_templates
 */
$total = count($rows);
?>

<?php if (!empty($form)): ?>
  <?php print $form ?>
<?php else: ?>
  <div class="item-list">
    <ul>
    <?php
      foreach ($rows as $i => $row):
        $class = '';
        $class .= ($i == 0) ? "views-row-first" : "";
        $class .= ($i == $total - 1) ? " views-row-last" : "";
      ?>
      <li class='<?php print $class ?>'><a href="<?php print $row->url; ?>"><?php print $row->link; ?></a>
        <?php if (!empty($options['count'])): ?>
          (<?php print $row->count?>)
        <?php endif; ?>
      </li>
    <?php endforeach; ?>
    </ul>
  </div>
<?php endif; ?>
