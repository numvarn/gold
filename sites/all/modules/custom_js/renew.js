$(document).ready(function() {
	var choice = $('input:radio[name=choice]');

	$('div.renew-display-1').empty();
	$('div.renew-display-end-1').empty();
	$('div.stake-show').empty();
	$('div.stake-show').append($('#edit-stake-1').val());
	$('div.renew-display-1').append($('#edit-start-date-1').val());
	$('div.renew-display-end-1').append($('#edit-end-date-1').val());

	choice.click(function(){

		var value = $('input:radio[name=choice]:checked').val();

		if (value == 1) {
			$('div.renew-display-1').empty();
			$('div.renew-display-end-1').empty();
			$('div.stake-show').empty();
			$('div.stake-show').append($('#edit-stake-1').val());
			$('div.renew-display-1').append($('#edit-start-date-1').val());
			$('div.renew-display-end-1').append($('#edit-end-date-1').val());

			/*record value*/
			$('#edit-renew-date').val($('#edit-start-date-stamp-1').val());
			$('#edit-stake').val($('#edit-stake-1').val());
		}
		else if (value == 2) {
			$('div.renew-display-1').empty();
			$('div.renew-display-end-1').empty();
			$('div.stake-show').empty();
			$('div.stake-show').append($('#edit-stake-2').val());
			$('div.renew-display-1').append($('#edit-start-date-2').val());
			$('div.renew-display-end-1').append($('#edit-end-date-2').val());

			/*record value*/
			$('#edit-renew-date').val($('#edit-start-date-stamp-2').val());
			$('#edit-stake').val($('#edit-stake-2').val());
		}
		else if(value == 3) {
			$('div.renew-display-1').empty();
			$('div.renew-display-end-1').empty();
			$('div.stake-show').empty();
			$('div.stake-show').append($('#edit-stake-3').val());
			$('div.renew-display-1').append($('#edit-start-date-3').val());
			$('div.renew-display-end-1').append($('#edit-end-date-3').val());

			/*record value*/
			$('#edit-renew-date').val($('#edit-start-date-stamp-3').val());
			$('#edit-stake').val($('#edit-stake-3').val());
		}
		else if(value == 4) {
			$('div.renew-display-1').empty();
			$('div.renew-display-end-1').empty();
			$('div.stake-show').empty();
			$('div.stake-show').append($('#edit-stake-4').val());
			$('div.renew-display-1').append($('#edit-start-date-4').val());
			$('div.renew-display-end-1').append($('#edit-end-date-4').val());

			/*record value*/
			$('#edit-renew-date').val($('#edit-start-date-stamp-4').val());
			$('#edit-stake').val($('#edit-stake-4').val());
		};

	});
});
