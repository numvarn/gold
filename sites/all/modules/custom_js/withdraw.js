$(document).ready(function() {
	$("#transaction-withdraw-form .new-cost :input").number_format({precision: 0});

	var inputs = $("#transaction-withdraw-form .check :input");
	var values = {};
	var stake = Number($('#edit-stake').val());
	var total_money = Number($('#edit-money-total').val());
	var remain_money = 0;

	inputs.click(function(){
		var total_cost = 0;
		var result_cost = 0;
		var id = '';
		var id_new_cost = '';

		/*clear new cost value*/
		$.each($("#transaction-withdraw-form .new-cost :input").serializeArray(), function(key, field) {
		  	var current_id = '#edit-'+field.name;
			$(current_id).val($.number_format(0, {precision: 0}));
		});

		/*clear weight value*/
		$.each($("#transaction-withdraw-form .weight-reduce :input").serializeArray(), function(key, field) {
		  	var current_id = '#edit-'+field.name;
			$(current_id).val('');
		});

		$.each($("#transaction-withdraw-form .check :input").serializeArray(), function(i, field) {
		    values[field.name] = field.value;
			id = "#edit-"+field.value+"-value";

			var detail_cost = Number($(id).val());

			/*set new cost value*/
			var current_new_cost = '#edit-'+field.value+"-new-cost";
			$(current_new_cost).val($.number_format(detail_cost, {precision: 0}));

			/*get detail cost and sum*/
			var current_new_cost_get =  $(current_new_cost).val();
			current_new_cost_get = Number(current_new_cost_get.replace(/,/g,''));

			/*sum total cost*/
			total_cost += current_new_cost_get;

			/*set default weight*/
			var weight_id = "#edit-"+field.value+"-weight";
			var weight_default_id = "#edit-"+field.value+"-weight-default";
			var default_weight = $(weight_default_id).val();

			$(weight_id).val(default_weight);
		});

		result_cost = total_cost + stake;
		remain_money = total_money - total_cost;

		$('div.money-str-html').empty();
		$('div.money-str-html').append($.number_format(remain_money, {precision: 0}));

		$('td.cost-display').empty();
		$('td.cost-display').append($.number_format(result_cost, {precision: 0}));

		$('#edit-reduce-cost').empty();
		$('#edit-reduce-cost').val(total_cost);
	});
});


/*set action for new-cost*/
$(document).ready(function(){

	var stake_cost = Number($('#edit-stake').val());
	var inputs = $("#transaction-withdraw-form .new-cost :input");
	var values = {};
	var customer_must_pay = 0;


	/*clean data*/
	//stake_cost = Number(stake_cost.replace(/,/g,''));

	inputs.keyup(function(){
		var result = 0;
		var tmp = 0;

		$.each($("#transaction-withdraw-form .new-cost :input").serializeArray(), function(i, field) {
		    values[field.name] = field.value;
			tmp = field.value;
			tmp = tmp.replace(/,/g,'');

			/*get id integer*/
			var str = field.name;
			var id = str.split("-");

			var check_box_id = "#edit-"+id[0]+"-"+id[0];

			var check = $(check_box_id).attr('checked') ? 1 : 0 ;

			if (check) {
				result += Number(tmp);
				customer_must_pay = result + stake_cost;
			};

		});

		customer_must_pay_html = $.number_format(customer_must_pay, {precision: 0});
		reduce_html = $.number_format(result, {precision: 0});

		$('td.cost-display').empty();
		$('td.cost-display').append($.number_format(customer_must_pay_html, {precision: 0}));

		$('#edit-reduce-cost').empty();
		$('#edit-reduce-cost').val(result);
	});

});

$(document).ready(function() {
	$("#transaction-adjust-form .new-cost :input").number_format({precision: 0});

	$('input:radio[name="adjust-detail"]').filter('[value="0"]').attr('checked', true);

	$("#transaction-adjust-form").hide();

	var choice = $('input:radio[name=adjust-detail]');

	choice.click(function(){
		var value = $('input:radio[name=adjust-detail]:checked').val();
		if (value == 0) {
			$('#transaction-withdraw-form').show();
			$("#transaction-adjust-form").hide();
		}
		else if(value == 1) {
			$('#transaction-withdraw-form').hide();
			$("#transaction-adjust-form").show();
		};
	});
});
