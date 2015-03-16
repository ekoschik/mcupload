$(document).ready(function() {

    var host = window.document.location.href.replace(/\/$/, '');

    var socket = io.connect(host);

    var $body = $('body');
    var $modalOverlay = $('.modal-overlay');
    var $modalContent = $('.modal-content');
    var $modalImage = $('.modal-image', $modalContent);
    var $imageContainer = $('.image-list > ul');

    $('.image').click(onImageClick);
    socket.on('screenshot', insertImage);

    function insertImage(imageRec) {
        console.log('got image rec: ' + imageRec);
        var $image = $('<li><div class="image" style="background-image: url(' + imageRec.url + ')"></div></li>');
        $('.image', $image).click(onImageClick);
        $image.prependTo($imageContainer);
    }

    function onImageClick(e) {
        e.stopPropagation();
        var bg = $(this).css('background-image');
        $modalImage.css('background-image', bg);
        $modalOverlay.removeClass('hidden');
        $modalContent.removeClass('hidden');
        $body.click(dismissImageOverlay);
        $modalOverlay.click(dismissImageOverlay);
    }

    function dismissImageOverlay(e) {
        $body.off('click', dismissImageOverlay);
        $modalOverlay.off('click', dismissImageOverlay);
        $modalOverlay.addClass('hidden');
        $modalContent.addClass('hidden');
    }

    function getBackgroundImage(element) {
        var bg = $(element).css('background-image');
        return bg.replace('url(', '').replace(')', '');
    }

});