class GameManager{

	isFogEnabled = false;

	gameStarted = true;
	camera;
	scene;
	renderer;
	gameObjects = [];
	busses = [];
	cars = [];
	logs = [];
	turtles = [];
	crabs = [];

	colliders = [];
	spotlights = [];

	// Starting player position
	startingPlayerPosition = new THREE.Vector3(5,0,1);
	startingLives = 3;
	currentLives = 3;
	currentScore = 0;

	scorePerTarget = 200;

	shadowEnabled = false;
	reflectionEnabled = false;

	MOVE_UP = 1;
	MOVE_DOWN = 0;

	// Scene properties
	scene_color = 0x000000;
	scene_color_alpha = 1;

	// Camera Properties
	camera_angle = 0;
	camera_range = 18;
	camera_speed = 0.2 * Math.PI/180;
	camera_target = new THREE.Vector3(5, 5, 0);
	camera_focal = 60;
	camera_near = 0.1;
	camera_far = 20000;

	// Lights
	light_am_color = 0xAAAAAA;
	// Fog
	fogColor = new THREE.Color(0xffffff);
	blackColor = new THREE.Color("rgb(113,112,117)");

	bus_speed = 0.1;
	car_speed = 0.06;
	log_speed = -0.06;
	turtle_speed = 0.06;

	possibleTargetPositions = [2,4,6,8];

	clippingPlanes = [new THREE.Plane( new THREE.Vector3( 1, 0, 0 )), new THREE.Plane( new THREE.Vector3( -1, 0, 0 ), 10)]

    cameraOptions = {
        ORBIT: 0,
        CONTROLLED: 1,
		THIRD_PERSON: 2,
		TOP_DOWN: 3,
		TOP_DOWN_ORTHOGRAPHIC: 4,
		STEREO: 5
      };

	selectedCamera = this.cameraOptions.STEREO;

	areLampsEnabled = false;
	collidersVisible = false;
	lensflareEnabled = true;

	selfPlay = true;

    constructor(){

	}
	cycle(){
		var timeElapsed = new Date().getTime();
		var timeDelta = timeElapsed - timePrevious;
		timePrevious = timeElapsed;
		requestAnimFrame(cycle);
		if(texturesLeft >0) return;
		update();
		renderScene();
	}

	initScene(){


	}

	render (currentTime) {
		if(!this.gameStarted)
			return;

		if(this.selectedCamera == this.cameraOptions.ORBIT) {
			console.log("Orbiting")
			this.camera = this.perspective_camera;
			this.camera_angle += this.camera_speed;
			this.camera.position.x = Math.cos(this.camera_angle) * this.camera_range;
			this.camera.position.y = Math.sin(this.camera_angle) * this.camera_range;
			this.camera.position.z = 5;
			this.camera.up.set(0, 0, 1);
			this.camera.lookAt(this.camera_target);
		} else if(this.selectedCamera == this.cameraOptions.STEREO) {
			console.log("Orbiting")
			this.camera = this.perspective_camera;
			this.camera_angle += this.camera_speed;
			this.camera.position.x = Math.cos(this.camera_angle) * this.camera_range;
			this.camera.position.y = Math.sin(this.camera_angle) * this.camera_range;
			this.camera.position.z = 5;
			this.controls.update();
		} else if(this.selectedCamera == this.cameraOptions.THIRD_PERSON){
			console.log("Third person")
			this.camera = this.perspective_camera;
			this.camera.position.x = this.player.position.x;
			this.camera.position.y = this.player.position.y - 5;
			this.camera.position.z = 5;
			this.camera.up.set(0, 0, 1);
			var lookPos = this.player.position;
			lookPos.z = 1;
			this.camera.lookAt(lookPos);
		} else if(this.selectedCamera == this.cameraOptions.TOP_DOWN){
			this.camera = this.perspective_camera;
			this.camera.position.x = 5;
			this.camera.position.y = 5;
			this.camera.position.z = 15;
			this.camera.up.set(0, 1, 0);
			this.camera.lookAt(this.camera_target);
		}else if(this.selectedCamera == this.cameraOptions.TOP_DOWN_ORTHOGRAPHIC) {
			this.camera = this.orthographic_camera;
			this.camera.position.x = 5;
			this.camera.position.y = 5;
			this.camera.position.z = 15;
			this.camera.up.set(0, 1, 0);
			this.camera.lookAt(this.camera_target);
		}

		if (this.shadowEnabled) {
			this.renderer.shadowMap.enabled = true;
			this.directionalLight.castShadow = true;
		} else {
			this.renderer.shadowMap.enabled = false;
			this.directionalLight.castShadow = false;
		}


		for(var obj of this.gameObjects){
			obj.render();
		}

		this.updateBusses();
		this.updateCars();
		this.updateLogs();
		this.updateTurtles(currentTime);
		
		var crabRotation = this.crabs[0].meshes[0].rotation.clone();
		this.crabs[0].meshes[0].rotation.setFromRotationMatrix( this.camera.matrix );
		this.player.render(currentTime);

		var isCollidingWithBus = this.checkCollidingBusses();
		var isCollidingWithCar = this.checkCollidingCars();

		if(isCollidingWithCar || isCollidingWithBus)
			this.resetPlayer();

		this.checkGroundCollision();

		if(this.target.boundingBox.intersectsBox(this.player.boundingBox) || this.target.boundingBox2.intersectsBox(this.player.boundingBox)){
			this.respawnPlayer();
			this.currentScore += this.scorePerTarget;
		}

		if (this.selectedCamera != this.cameraOptions.STEREO) {
			this.displayText();
		} else {
			this.clearText();
		}

		if (this.reflectionEnabled) {
			this.mirrorRect3.visible = false;
			this.mirrorRect3Camera.update( this.renderer, this.scene );
			this.mirrorRect3.visible = true;
		}

		this.renderer.render(this.scene, this.camera);

		if (this.selectedCamera == this.cameraOptions.STEREO) {
			this.effect.render(this.scene, this.camera );
		}
    };

	webGLStart(){
		// New renderer
		this.renderer = new THREE.WebGLRenderer({antialias: true});
		this.renderer.shadowMapType = THREE.PCFSoftShadowMap;
		this.renderer.setSize(window.innerWidth, window.innerHeight);
		this.renderer.setClearColor(this.scene_color, this.scene_color_alpha);

		// Add the renderer to the DOM
		document.body.appendChild(this.renderer.domElement);

		// Create the scene
		this.scene = new THREE.Scene();
 
		// ***** Clipping setup (renderer): *****
		this.renderer.localClippingEnabled = true;

		// ***** Fog *****
		this.toggleFog();

		// Set some camera defaults
		this.perspective_camera = new THREE.PerspectiveCamera(this.camera_focal, window.innerWidth/window.innerHeight,this.camera_near, this.camera_far);
		this.perspective_camera.position.set(0, this.camera_range, 0);
		this.perspective_camera.useQuaternion = true;
		this.perspective_camera.lookAt(this.camera_target);

		// Set some camera defaults
		this.orthographic_camera = new THREE.OrthographicCamera( window.innerWidth / - 120, window.innerWidth / 120, window.innerHeight / 120, window.innerHeight / - 120,this.camera_near, this.camera_far);
		this.orthographic_camera.position.set(0, this.camera_range, 0);
		this.orthographic_camera.useQuaternion = true;
		this.orthographic_camera.lookAt(this.camera_target);
		this.scene.add(this.orthographic_camera);

		// Stereo effect
		this.effect = new StereoEffect( this.renderer );
		this.effect.setSize( window.innerWidth, window.innerHeight );

		// Add abbient light
		var am_light = new THREE.AmbientLight(this.light_am_color); // soft white light
		this.scene.add(am_light);

		// LENS FLARE
		var textureLoader = new THREE.TextureLoader();

		var texture0 = textureLoader.load( "textures/lensflare/lensflare0.png" );
		var texture3 = textureLoader.load( "textures/lensflare/lensflare3.png" );

		this.lensflare = new THREE.Lensflare();
		this.lensflare.addElement( new THREE.LensflareElement( texture0, 300, 0 ) );
		this.lensflare.addElement( new THREE.LensflareElement( texture3, 60, 0.6 ) );
		this.lensflare.addElement( new THREE.LensflareElement( texture3, 70, 0.7 ) );
		this.lensflare.addElement( new THREE.LensflareElement( texture3, 120, 0.9 ) );
		this.lensflare.addElement( new THREE.LensflareElement( texture3, 70, 1 ) );
		this.lensflare.visible = this.lensflareEnabled;
		this.directionalLight = new THREE.SpotLight(0xDDDDDD,1);
		this.directionalLight.position.set(12, 12 ,6);

		this.directionalLight.add(this.lensflare);
		this.scene.add(this.directionalLight);

		// Add directional light
		this.createSpotlights();
		this.createGameObjects();
		this.toggleSpotlights();

		// Skybox
		const loader = new THREE.CubeTextureLoader();
		const texture = loader.load([
			"./textures/ely_cloudtop/cloudtop_lf.png",
			"./textures/ely_cloudtop/cloudtop_rt.png",
			"./textures/ely_cloudtop/cloudtop_up.png",
			"./textures/ely_cloudtop/cloudtop_dn.png",
			"./textures/ely_cloudtop/cloudtop_ft.png",
			"./textures/ely_cloudtop/cloudtop_bk.png",
		]);

		let fragmentShader = `
		uniform samplerCube tCube;
		varying vec3 vWorldDirection;
		void main() {
			vec4 texColor = textureCube( tCube, vec3( vWorldDirection.xzy ) );
			gl_FragColor = mapTexelToLinear( texColor );
		}`;

		let vertexShader = `
		varying vec3 vWorldDirection;
		#include <common>
		void main() {
			vWorldDirection = transformDirection( position, modelMatrix );
        	vec4 mvPosition = modelViewMatrix * vec4( vec3(position), 1.0 );
			gl_Position = projectionMatrix * mvPosition;
		}`;

		var shaderMaterial = new THREE.ShaderMaterial( {
			fragmentShader: fragmentShader,
			vertexShader: vertexShader,
			uniforms: {
				tCube: { value: texture },
			},
			depthWrite: false,
			side: THREE.BackSide
		} );
		this.scene.add(new THREE.Mesh(new THREE.BoxGeometry( 999, 999, 999), shaderMaterial));

		// Gyro controls
		this.controls = new DeviceOrientationControls( this.perspective_camera );

		// Keyboard listener
		this.toggleColliders();
		document.addEventListener("keydown", (event)=>{this.onDocumentKeyDown(event)}, false);

		// Render loop
		this.renderer.setAnimationLoop((currentTime)=>{this.render(currentTime);});

	}
	createGameObjects(){
		this.gameObjects.push(new Grass(new THREE.Vector3(0,0,0)));
		this.gameObjects.push(new Grass(new THREE.Vector3(0,6,0)));
		this.gameObjects.push(new GrassBlock(new THREE.Vector3(0,12,0)));
		this.gameObjects.push(new GrassBlock(new THREE.Vector3(2,12,0)));
		this.gameObjects.push(new GrassBlock(new THREE.Vector3(4,12,0)));
		this.gameObjects.push(new GrassBlock(new THREE.Vector3(6,12,0)));
		this.gameObjects.push(new GrassBlock(new THREE.Vector3(8,12,0)));
		this.target = new Crystal(new THREE.Vector3(0,12,0));
		this.repositionTarget();
		this.gameObjects.push(this.target);
		this.gameObjects.push(new Road(new THREE.Vector3(0,3,0)));
		this.gameObjects.push(new River(new THREE.Vector3(0,9,0)));

		this.crabs.push(new Crab(new THREE.Vector3(0.5,12,0.8)));
		this.gameObjects.push(this.crabs[0]);
		this.player = new Frog(this.startingPlayerPosition, this.selfPlay);
		
		this.scene.add(this.player.meshes[0]);
		this.scene.add(this.player.meshes[1]);
		this.scene.add(this.player.meshes[2]);
		this.colliders.push(this.player.colliders[0])
		this.scene.add(this.player.colliders[0])

		this.createBusses();
		this.createCars();
		this.createTurtles();
		this.createLogs();
		for(var obj of this.gameObjects){
			for(var mesh of obj.meshes){
				this.scene.add(mesh);	
			}
			if(obj.colliders){
				for(var collider of obj.colliders){
					console.log("Has collider");
					this.colliders.push(collider);
					this.scene.add(collider);	
				}
			}
		}
		
		this.createReflectionPlane()
	}

	createReflectionPlane() {  
		//var geometry =  new THREE.SphereGeometry( 3, 64, 64 );
		var geometry =  new THREE.PlaneGeometry(10,7, 32 );
		this.mirrorRect3Camera = new THREE.CubeCamera(1, 10000, 1024 );

        this.boxHelper = new THREE.BoxHelper( this.mirrorRect3Camera, 0xffff00 );
        this.scene.add(this.boxHelper);
		this.scene.add( this.mirrorRect3Camera );
		var mirrorRectMaterial = new THREE.MeshBasicMaterial({ 
		envMap: this.mirrorRect3Camera.renderTarget,
		//reflectivity: 0.9,
		transparent: true,
		//side:THREE.DoubleSide, 
		opacity: 0.8,
		color: 0xaaaaaa,
		});
		this.mirrorRect3 = new THREE.Mesh( geometry, mirrorRectMaterial );
		this.mirrorRect3.position.set(5,9,0.41);
		this.mirrorRect3Camera.rotation.x = 1.5;
		this.mirrorRect3Camera.position.set(0,9,1);
		this.scene.add(this.mirrorRect3);
	}
  
	onDocumentKeyDown(event) {
		var keyCode = event.which;
		console.log(keyCode)
		switch(keyCode){
			case 39:
				if(this.player.position.x < 9) this.player.currentDirection = this.player.direction.RIGHT;
				break;
			case 37:
				if(this.player.position.x > 0) this.player.currentDirection = this.player.direction.LEFT;
				break;
			case 38:
				if(this.player.position.y < 12) this.player.currentDirection = this.player.direction.UP;
				break;
			case 40:
				if(this.player.position.y > 0) this.player.currentDirection = this.player.direction.DOWN;
				break;
			case 49:
				this.selectedCamera = this.cameraOptions.ORBIT;
				this.renderer.setSize(window.innerWidth, window.innerHeight);
				break;
			case 50:
				this.selectedCamera = this.cameraOptions.THIRD_PERSON;
				this.renderer.setSize(window.innerWidth, window.innerHeight);
				break;
			case 51:
				this.selectedCamera = this.cameraOptions.TOP_DOWN;
				this.renderer.setSize(window.innerWidth, window.innerHeight);
				break;
			case 52:
				this.selectedCamera = this.cameraOptions.TOP_DOWN_ORTHOGRAPHIC;
				this.renderer.setSize(window.innerWidth, window.innerHeight);
				break;
			case 53:
				this.selectedCamera = this.cameraOptions.STEREO;
				break;
			case 81:
				this.areLampsEnabled = !this.areLampsEnabled;
				this.toggleSpotlights();
				break;
			case 82:
				this.resetPlayer();
				break;
			case 70: // fog
				this.isFogEnabled = !this.isFogEnabled;
				this.toggleFog();
			
			case 65: // Visible colliders
				this.collidersVisible = !this.collidersVisible;
				this.toggleColliders();
			
			case 83: // Lens flare
				this.lensflareEnabled = !this.lensflareEnabled;
				this.lensflare.visible = this.lensflareEnabled;
				break;

			case 77:
				this.shadowEnabled = !this.shadowEnabled;
				break;
			case 78:
				this.reflectionEnabled = !this.reflectionEnabled;
				break;
		}
		
	};

	toggleSpotlights(){
		for(var spotL of this.spotlights){
			spotL.toggleLight(this.areLampsEnabled);
		}

		this.directionalLight.intensity = !this.areLampsEnabled ? 1 : 0;
	}

	toggleColliders(){
		for(var collider of this.colliders){
			collider.visible = this.collidersVisible;
		}
		
	}

	createBusses(){
		var offset = Math.random() * (5 - 0.1) + 0.1;
		this.busses.push(new Bus(new THREE.Vector3(-5,4,1),this.clippingPlanes));
		this.busses.push(new Bus(new THREE.Vector3(-8-offset,4,1),this.clippingPlanes));
		for(var bus of this.busses){
			this.gameObjects.push(bus);
		}
	}
	createCars(){
		this.cars.push(new Car(new THREE.Vector3(11,2,1), this.clippingPlanes));

		var offset = Math.random() * (2 - 0.1) + 0.1;
		this.cars.push(new Car(new THREE.Vector3(14+offset,2,1), this.clippingPlanes));
		
		var offset = Math.random() * (2 - 0.1) + 0.1;
		this.cars.push(new Car(new THREE.Vector3(17+offset,2,1), this.clippingPlanes));
		for(var car of this.cars){
			this.gameObjects.push(car);
		}
	}


	createLogs(){
		var offset = Math.random() * (2 - 0.1) + 0.1;
		this.logs.push(new Log(new THREE.Vector3(11+offset,11,1),this.clippingPlanes));

		var offset = Math.random() * (2 - 0.1) + 0.1;
		this.logs.push(new Log(new THREE.Vector3(14+offset,11,1),this.clippingPlanes));
		
		var offset = Math.random() * (2 - 0.1) + 0.1;
		this.logs.push(new Log(new THREE.Vector3(17+offset,11,1),this.clippingPlanes));


		var offset = Math.random() * (2 - 0.1) + 0.1;
		this.logs.push(new Log(new THREE.Vector3(11+offset,7,1),this.clippingPlanes));

		var offset = Math.random() * (2 - 0.1) + 0.1;
		this.logs.push(new Log(new THREE.Vector3(14+offset,7,1),this.clippingPlanes));
		
		var offset = Math.random() * (2 - 0.1) + 0.1;
		this.logs.push(new Log(new THREE.Vector3(17+offset,7,1),this.clippingPlanes));


		var offset = Math.random() * (2 - 0.1) + 0.1;
		this.logs.push(new Log(new THREE.Vector3(11+offset,9,1),this.clippingPlanes));

		var offset = Math.random() * (2 - 0.1) + 0.1;
		this.logs.push(new Log(new THREE.Vector3(14+offset,9,1),this.clippingPlanes));
		
		var offset = Math.random() * (2 - 0.1) + 0.1;
		this.logs.push(new Log(new THREE.Vector3(17+offset,9,1),this.clippingPlanes));

		for(var log of this.logs){
			this.gameObjects.push(log);
		}
	}

	repositionTarget(){
		var pos = this.possibleTargetPositions[Math.floor(Math.random()*this.possibleTargetPositions.length)];
		this.target.position = new THREE.Vector3(pos,12,0);
		this.target.updatePosition();
	}

	createSpotlights(){

		this.spotlights.push(new Spotlight(new THREE.Vector3(0.5,2,5)));
		this.spotlights.push(new Spotlight(new THREE.Vector3(9.5,2,5)));
		this.spotlights.push(new Spotlight(new THREE.Vector3(0.5,5,5)));
		this.spotlights.push(new Spotlight(new THREE.Vector3(9.5,5,5)));

		this.spotlights.push(new Spotlight(new THREE.Vector3(0.5,8,5)));
		this.spotlights.push(new Spotlight(new THREE.Vector3(9.5,8,5)));
		this.spotlights.push(new Spotlight(new THREE.Vector3(0.5,11,5)));
		this.spotlights.push(new Spotlight(new THREE.Vector3(9.5,11,5)));
		for(var spotL of this.spotlights){
			this.scene.add(spotL.targetObject); 
			this.scene.add(spotL.spot_light);
		}
	}

	createTurtles(){
		this.turtles.push(new Turtle(new THREE.Vector3(-3,10,1),this.clippingPlanes));
		this.turtles.push(new Turtle(new THREE.Vector3(-4.5,10,1),this.clippingPlanes));
		this.turtles.push(new Turtle(new THREE.Vector3(-6,10,1),this.clippingPlanes));


		this.turtles.push(new Turtle(new THREE.Vector3(-9,10,1),this.clippingPlanes));
		this.turtles.push(new Turtle(new THREE.Vector3(-10.5,10,1),this.clippingPlanes));
		this.turtles.push(new Turtle(new THREE.Vector3(-12,10,1),this.clippingPlanes));


		var offset = Math.random() * (2 - 0.1) + 0.1;
		this.turtles.push(new Turtle(new THREE.Vector3(-3-offset,8,1),this.clippingPlanes));
		this.turtles.push(new Turtle(new THREE.Vector3(-4.5-offset,8,1),this.clippingPlanes));
		this.turtles.push(new Turtle(new THREE.Vector3(-6-offset,8,1),this.clippingPlanes));

		this.turtles.push(new Turtle(new THREE.Vector3(-9-offset,8,1),this.clippingPlanes));
		this.turtles.push(new Turtle(new THREE.Vector3(-10.5-offset,8,1),this.clippingPlanes));
		this.turtles.push(new Turtle(new THREE.Vector3(-12-offset,8,1),this.clippingPlanes));


		for(var turtle of this.turtles){
			this.gameObjects.push(turtle);
		}
	}
	checkCollidingBusses(){
		var isColliding = false;
		for(var bus of this.busses){
			isColliding = isColliding || bus.boundingBox.intersectsBox(this.player.boundingBox);
		}
		return isColliding;
	}
	checkCollidingCars(){
		var isColliding = false;
		for(var car of this.cars){
			isColliding = isColliding || car.boundingBox.intersectsBox(this.player.boundingBox);
		}
		return isColliding;
	}

	checkGroundCollision(){
		var hitRiver = false;
		var isOnGround = false;
		var isOnLog = false;
		var isOnTurtle = false;
		for(var gameObject of this.gameObjects){
			if(gameObject.TAG){
				var isColliding = this.player.bottomBoundingBox.intersectsBox(gameObject.boundingBox);
				if(isColliding){
					if(gameObject.TAG == "RIVER"){
						hitRiver = true;
					}else if(gameObject.TAG == "GROUND"){
						hitRiver = false;
						isOnLog = false;
						isOnGround = true;
						isOnTurtle = false;
					}else if(gameObject.TAG == "LOG"){
						isOnLog = true;
					}else if(gameObject.TAG == "TURTLE"){
						if(gameObject.isUnderWater){
							hitRiver = true;
							isOnTurtle = false;
						}else{
							hitRiver = false;
							isOnTurtle = true;
						}
					}
				}
			}
		}

		if(isOnLog){
			this.player.currentTag = "FLOATING";
			this.player.floatingMovingSpeed = this.log_speed;
		}else if(isOnTurtle){
			this.player.currentTag = "FLOATING";
			this.player.floatingMovingSpeed = this.turtle_speed;
		}else if (hitRiver && !isOnGround){
			this.resetPlayer();
		}else{
			this.player.currentTag = "GROUND";
		}

	}
	updateBusses(){
		for(var bus of this.busses){
			bus.position.x += this.bus_speed;
			if(bus.position.x > 12){
				bus.position.x = -4;
			}
			bus.updatePosition();
		}
	}

	updateCars(){
		for(var car of this.cars){
			car.position.x -= this.car_speed;
			if(car.position.x < -1){
				car.position.x = 11;
			}
			car.updatePosition();
		}
	}

	updateLogs(){
		for(var log of this.logs){
			log.position.x += this.log_speed;
			if(log.position.x < -1){
				log.position.x = 11;
			}
			log.updatePosition();
		}
	}

	updateTurtles(currentTime){
		for(var turtle of this.turtles){
			turtle.position.x += this.turtle_speed;
			if(turtle.position.x > 10){
				turtle.position.x = -3;
			}
			turtle.updatePosition(currentTime);
		}
	}

	respawnPlayer(){
		this.player.reset ();
		this.player.position = this.startingPlayerPosition;
	}

	resetPlayer(){
		this.currentLives -= 1;
		if(this.selfPlay){
			this.currentLives = 1000;
		}
		if(!this.gameStarted){
			this.respawnPlayer();
			this.currentLives = this.startingLives;
			this.gameStarted = true;
			this.score = 0;
		}

		if(this.currentLives <= 0){
			this.gameStarted = false;
		}else {
			this.respawnPlayer();
		}
	}

	toggleFog(){
		if(this.isFogEnabled){
			this.scene.fog = new THREE.Fog(this.fogColor, 0.0025, 20);
		}else{
			this.scene.background = this.blackColor;
			this.scene.fog = null;
		}
	}
	displayText(){
		document.getElementById("score").innerHTML = "Score: " + this.currentScore;
		document.getElementById("lives").innerHTML = "Lives: " + this.currentLives;
		document.getElementById("fog").innerHTML = "Fog (f): " + this.isFogEnabled;
		document.getElementById("camera").innerHTML = "Selected camera: " + this.selectedCamera;
		document.getElementById("lamps").innerHTML = "Side lamps (q): " + (this.areLampsEnabled);
		document.getElementById("colliders").innerHTML = "Visible Colliders (a): " + (this.collidersVisible);
		document.getElementById("lensflare").innerHTML = "Lensflare (s): " + (this.lensflareEnabled);
		document.getElementById("shadow").innerHTML = "Shadow (m): " + (this.shadowEnabled);
		document.getElementById("reflection").innerHTML = "Reflection (n): " + (this.reflectionEnabled);
	}

	clearText() {
		document.getElementById("score").innerHTML = "";
		document.getElementById("lives").innerHTML = "";
		document.getElementById("fog").innerHTML = "";
		document.getElementById("camera").innerHTML = "";
		document.getElementById("lamps").innerHTML = "";
		document.getElementById("colliders").innerHTML = "";
		document.getElementById("lensflare").innerHTML = "";
		document.getElementById("shadow").innerHTML = "";
		document.getElementById("reflection").innerHTML = "";
	}
}

var StereoEffect = function ( renderer ) {
	var _stereo = new THREE.StereoCamera();
	_stereo.aspect = 0.5;
	var size = new THREE.Vector2();

	this.setEyeSeparation = function ( eyeSep ) {
		_stereo.eyeSep = eyeSep;
	};

	this.setSize = function ( width, height ) {
		renderer.setSize(width, height );
	};

	this.render = function ( scene, camera ) {
		scene.updateMatrixWorld();
		if ( camera.parent === null )
			camera.updateMatrixWorld();

		_stereo.update( camera );
		renderer.getSize( size );

		if (renderer.autoClear)
			renderer.clear();
		renderer.setScissorTest( true );
		renderer.setScissor( 0, 0, size.width / 2, size.height );
		renderer.setViewport( 0, 0, size.width / 2, size.height );
		renderer.render( scene, _stereo.cameraL );
		renderer.setScissor( size.width / 2, 0, size.width / 2, size.height );
		renderer.setViewport( size.width / 2, 0, size.width / 2, size.height );
		renderer.render( scene, _stereo.cameraR );
		renderer.setScissorTest( false );
	};
};

var DeviceOrientationControls = function ( object ) {
	var scope = this;
	this.object = object;
	this.object.rotation.reorder( 'YXZ' );
	this.enabled = true;
	this.deviceOrientation = {};
	this.screenOrientation = 0;
	this.alphaOffset = 0; // radians

	var onDeviceOrientationChangeEvent = function ( event ) {
		scope.deviceOrientation = event;
	};

	var onScreenOrientationChangeEvent = function () {
		scope.screenOrientation = window.orientation || 0;
	};

	// The angles alpha, beta and gamma form a set of intrinsic Tait-Bryan angles of type Z-X'-Y''
	var setObjectQuaternion = function () {
		var zee = new THREE.Vector3( 0, 0, 1 );
		var euler = new THREE.Euler();
		var q0 = new THREE.Quaternion();
		var q1 = new THREE.Quaternion( - Math.sqrt( 0.5 ), 0, 0, Math.sqrt( 0.5 ) ); // - PI/2 around the x-axis

		return function ( quaternion, alpha, beta, gamma, orient ) {
			euler.set( beta, alpha, - gamma, 'YXZ' ); // 'ZXY' for the device, but 'YXZ' for us
			quaternion.setFromEuler( euler ); // orient the device
			quaternion.multiply( q1 ); // camera looks out the back of the device, not the top
			quaternion.multiply( q0.setFromAxisAngle( zee, - orient ) ); // adjust for screen orientation
		};
	}();

	this.connect = function () {
		onScreenOrientationChangeEvent(); // run once on load
		// iOS 13+
		if ( window.DeviceOrientationEvent !== undefined && typeof window.DeviceOrientationEvent.requestPermission === 'function' ) {
			window.DeviceOrientationEvent.requestPermission().then( function ( response ) {
				if ( response == 'granted' ) {
					window.addEventListener( 'orientationchange', onScreenOrientationChangeEvent, false );
					window.addEventListener( 'deviceorientation', onDeviceOrientationChangeEvent, false );
				}
			} ).catch( function ( error ) {
				console.error( 'THREE.DeviceOrientationControls: Unable to use DeviceOrientation API:', error );
			} );
		} else {
			window.addEventListener( 'orientationchange', onScreenOrientationChangeEvent, false );
			window.addEventListener( 'deviceorientation', onDeviceOrientationChangeEvent, false );
		}
		scope.enabled = true;
	};

	this.disconnect = function () {
		window.removeEventListener( 'orientationchange', onScreenOrientationChangeEvent, false );
		window.removeEventListener( 'deviceorientation', onDeviceOrientationChangeEvent, false );
		scope.enabled = false;
	};

	this.update = function () {
		if ( scope.enabled === false ) return;
		var device = scope.deviceOrientation;
		if ( device ) {
			var alpha = device.alpha ? _Math.degToRad( device.alpha ) + scope.alphaOffset : 0; // Z
			var beta = device.beta ? _Math.degToRad( device.beta ) : 0; // X'
			var gamma = device.gamma ? _Math.degToRad( device.gamma ) : 0; // Y''
			var orient = scope.screenOrientation ? _Math.degToRad( scope.screenOrientation ) : 0; // O
			setObjectQuaternion( scope.object.quaternion, alpha, beta, gamma, orient );
		}
	};

	this.dispose = function () {
		scope.disconnect();
	};
	this.connect();
};