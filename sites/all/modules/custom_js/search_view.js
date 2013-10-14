/**
 *Hide content area when open page in first time
 */
 
$(document).ready(function() {
	$(".view-content").hide();
});

/*Check action when user click*/ 
$(document).ready(function(){

	$("#edit-submit-transaction-all").click(function(){
		$(".view-content").fadeTo(6000);
	});

});