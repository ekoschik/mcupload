$(document).ready(function() {
    var $body = $('body');
    var $modalOverlay = $('.modal-overlay');
    var $modalContent = $('.modal-content');
    var $modalImage = $('.modal-image', $modalContent);

    $('.image').click(onImageClick);

    function onImageClick(e) {
        e.stopPropagation();
        var bg = getBackgroundImage(this);
        console.log('got background ' + bg);
        var bg = $(this).css('background-image');
        $modalImage.css('background-image', bg);
        $modalOverlay.removeClass('hidden');
        $modalContent.removeClass('hidden');
        $body.click(dismissImageOverlay);
    }

    function dismissImageOverlay(e) {
        $body.off('click', dismissImageOverlay);
        $modalOverlay.addClass('hidden');
        $modalContent.addClass('hidden');
    }

    function getBackgroundImage(element) {
        var bg = $(element).css('background-image');
        return bg.replace('url(', '').replace(')', '');
    }
});