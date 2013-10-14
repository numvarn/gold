<div class="node-wrapper">
	
	<div id="node-<?php print $node->nid; ?>" class="node post<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?> clear-block">
		
		<?php print $node->content['trasaction_renew']['#value'] ?>
		
		<?php print $node->content['transaction_node_attach_node_content_1']['#value'] ?>
		
		<?php print $node->content['trasaction_loss_status']['#value'] ?>
		
		<?php print $node->content['trasaction_detail_type']['#value'] ?>
		
		<?php print $node->content['trasaction_stake_calculate']['#value'] ?>
		
		<?php print $node->content['trasaction_renew_parent']['#value'] ?>
		
  		<?php if ($links): ?>
    		<div class="postmeta"><?php print $links; ?></div>
  		<?php endif; ?>
	</div>
	
<!-- 	
	<?php if ($terms): ?>
    	<div class="terms terms-inline"><?php print $terms ?></div>
  	<?php endif;?> 
-->
</div>