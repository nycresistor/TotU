var TOTU = (function() {
	var states = ["off", "preview", "record", "postview" ];
	var state = 0;
	var exports;
	var video;
	var app;
	var ctx;
	var timer;
	var controllers = {};
	var photos = [];

	function init() {
		navigator.getUserMedia =  navigator.getUserMedia ||
			navigator.webkitGetUserMedia ||
			navigator.mozGetUserMedia ||
			navigator.msGetUserMedia;

		app = $('#app')[0];

		if ($('video').length) {
			ctx = app.getContext('2d');
			video = $('video')[0];

			controllers['preview'] = preview;
			controllers['record'] = record;
			controllers['postview'] = postview;

			$('#btn-photo').click(nextState);
			$('#btn-send').click(send);

			nextState();
		}
		else {
//			startVisualizer();
		}
	}

	// Progresses through the different video states.  From preview, to capture, to postview and back.
	function nextState() {
		state++;
		
		// We never go back to the off state..
		if (state >= states.length)
			state = 1;

		// Following a MVCish structure..
		changeView(states[state]);
		fireController(states[state]);
	}

	// Updates the display according to the state and any current model data..
	// Basically just two states, recording or not...
	function changeView(state, model) {
		if (state == "record") {
			$('#flash').show();
			$('#btn-bar').hide();
			$('#countdown-bar').show();

			if (!model) {
				$('#countdown-7').removeClass('active');
			}
			else {
				$('#countdown-' + (7-model)).removeClass('active');
				$('#countdown-' + (8-model)).addClass('active');
			}

			if (typeof(model) != "undefined") {
				var sound = model > 4 ? $('#beep')[0] : $('#click')[0];
				sound.currentTime = 0;
				sound.play();

				if (model < 5) {
					$('#flash').attr('class','nofade').addClass('opaque');
					window.setTimeout(
						function() { 
							$('#flash').attr('class','fade').addClass('transparent'); 
						}, 100)
					;
				}
			}
		}
		else {
			$('#btn-bar').show();
			$('#countdown-bar').hide();
			$('#flash').hide();

			setBtnEnabled('#btn-photo', true);
			setBtnEnabled('#btn-send', state == "postview" && model !== false);
		}
	}

	// Sets a button as enabled or disabled..
	function setBtnEnabled(id, enabled) {
		var src = $(id).attr('src').replace('-inactive','');

		$(id).attr('src', enabled ? src : src.replace('.png','-inactive.png'));
		$(id).attr('disabled', !enabled);
	}

	function fireController(state) {
		return controllers[state]();
	}


	// Starts the camera using the WebRTC API and sends it to a hidden video element.  We pull 
	// the images from that into a canvas where we can capture them and turn them into data urls..
	function preview() {

		// Copies the current video frame to the canvas at 30 FPS
		function startTimer() {
			timer = setInterval(function() { ctx.drawImage(video,0,0,320,240); }, 33);
		}

		// If we're going from postview back to preview make sure we clear that timer
		if (timer) {
			clearInterval(timer);
			timer = 0;
		}

		// If we haven't setup the video, do that now
		if (video.readyState == 0) {
			navigator.getUserMedia({video:true}, 
				function(stream) {
					video.src = window.URL.createObjectURL(stream);
					startTimer();
				},
				function(err) { console.log(err) }
			);
		}
		// Otherwise restart the timer that copies to canvas
		else {
			startTimer();
		}
	}

	// Runs through a countdown timer and then captures 4 frames of the video
	function record() {
		photos = []; 		// Clear any previous photos
		var count = 8;

		var ctimer = setInterval(function() {
			count--;
			
			if (count) {
				if (count < 5) {
					var img = new Image();
					img.src = app.toDataURL('image/jpeg');
					photos.push(img);
				}

				changeView("record", count);
			}
			else {
				clearInterval(ctimer);
				clearInterval(timer);  // Stop the camera too

				nextState();
			}
		}, 850);
	}

	// Show the captured frames that will be saved..
	function postview() {
		var idx = 0;

		timer = setInterval(function() {
			ctx.drawImage(photos[idx],0,0);
			idx++;
			if (idx >= photos.length) 
				idx=0;
		}, 850);
	}

	// Save the frames to firebase
	function send() {
		var firebase = new Firebase('https://totu.firebaseio.com/submissions');
		firebase.push(
			{
				'submitted': Firebase.ServerValue.TIMESTAMP,
				'photos': photos.map(function(a) { return a.src })
			}, 
			function(error) {
				if (error) console.log(error);
				else {
					$('#success').show();
					changeView('postview', false)
				}
			}
		);
	}

	function startVisualizer() {
		var scene = new THREE.Scene();
		var camera = new THREE.PerspectiveCamera( 75, $('#app').width() / $('#app').height(), 0.1, 1000 );
		var renderer = new THREE.WebGLRenderer({ canvas: app });
		var controls = new THREE.TrackballControls( camera );

		controls.rotateSpeed = 1.0;
		controls.zoomSpeed = 1.2;
		controls.panSpeed = 0.8;

		controls.noZoom = false;
		controls.noPan = false;

		controls.staticMoving = true;
		controls.dynamicDampingFactor = 0.3;

		controls.keys = [ 65, 83, 68 ];

		var geometry = new THREE.BoxGeometry(10,1,1);
//		var texture = new THREE.Texture($('#texture')[0]);
//		var textureContext = $('#texture')[0].getContext('2d');
//		texture.minFilter = THREE.LinearFilter;
//		texture.magFilter = THREE.LinearFilter;
		var material = new THREE.MeshBasicMaterial( { /*map: texture, overdraw: 0.5*/ } );
		var cube = new THREE.Mesh( geometry, material );
		scene.add( cube );
		camera.position.z = 5;


		function render() {
			requestAnimationFrame(render);
//				if (video.readyState === video.HAVE_ENOUGH_DATA) {
//					textureContext.drawImage(video, 0, 0, 320, 240);
//					texture.needsUpdate = true;
//				}
				controls.update();
				renderer.render(scene, camera);
		}
		
		render();
	}

	$(window).load(init);

	return exports;
}());

//$(window).load(function() { startThreeJs(); });

