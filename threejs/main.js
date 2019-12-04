
class GameManager{
	camera;
	scene;
	renderer;
	gameObjects = [];
	busses = [];
	cars = [];
	logs = [];

	MOVE_UP = 1;
	MOVE_DOWN = 0;

	// Scene properties
	scene_color = 0x000000;
	scene_color_alpha = 1;

	// Camera Properties
	camera_angle = 0;
	camera_range = -12;
	camera_speed = 0.5 * Math.PI/180;
	camera_target = new THREE.Vector3(5, 7, 0);
	camera_focal = 60;
	camera_near = 0.1;
	camera_far = 50;

	// Lights
	light_am_color = 0xAAAAAA;
	light_spot_color = 0xDDDDDD;
	light_spot_intensity = 0.7;
	light_spot_position = {x: 5, y: 10, z: 6}
	light_spot_camera_near = 0.5;
	light_spot_shadow_darkness = 0.35;


	bus_speed = 0.1;
	car_speed = 0.06;
	log_speed = 0.06;

	possibleTargetPositions = [0,2,4,6,8];
    constructor(){
	}
	cycle(){
		timeElapsed = new Date().getTime();
		timeDelta = timeElapsed - timePrevious;
		timePrevious = timeElapsed;
		requestAnimFrame(cycle);
		if(texturesLeft >0) return;
		update();
		renderScene();
	}

	initScene(){


	}
	render () {
		this.camera_angle += this.camera_speed;
		this.camera.position.x = Math.cos(this.camera_angle) * this.camera_range;
		this.camera.position.y = Math.sin(this.camera_angle) * this.camera_range;
		this.camera.position.z = 5;
		this.camera.up.set(0, 0, 1);
		this.camera.lookAt(this.camera_target);

		for(var obj of this.gameObjects){
			obj.render();
		}

		this.updateBusses();
		this.updateCars();
		this.updateLogs();
		
		this.renderer.render(this.scene, this.camera);
	};


	webGLStart(){
		// New renderer
		this.renderer = new THREE.WebGLRenderer({antialias: true});
		this.renderer.shadowMap.enabled = true;
		this.renderer.shadowMapType = THREE.PCFSoftShadowMap;
		this.renderer.setSize(window.innerWidth, window.innerHeight);
		this.renderer.setClearColor(this.scene_color, this.scene_color_alpha);

		// Add the renderer to the DOM
		document.body.appendChild(this.renderer.domElement);

		// Create the scene
		this.scene = new THREE.Scene();

		// Set some camera defaults

		this.camera = new THREE.PerspectiveCamera(this.camera_focal, window.innerWidth/window.innerHeight,this.camera_near, this.camera_far);
		this.camera.position.set(0, this.camera_range, 0);
		this.camera.useQuaternion = true;
		this.camera.lookAt(this.camera_target);

		// Add abbient light
		var am_light = new THREE.AmbientLight(this.light_am_color); // soft white light
		this.scene.add(am_light);

		// Add directional light
		this.spot_light = new THREE.SpotLight(this.light_spot_color, this.light_spot_intensity);
		this.spot_light.position.set(this.light_spot_position.x, this.light_spot_position.y, this.light_spot_position.z);
		this.spot_light.target = this.scene;
		this.spot_light.castShadow = true;
		this.spot_light.receiveShadow = true;
		this.spot_light.shadowDarkness = this.light_spot_shadow_darkness;
		this.spot_light.shadowCameraNear	= this.light_spot_camera_near;		
		this.scene.add(this.spot_light);


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
		this.createBusses();
		this.createCars();
		this.createLogs();

		for(var obj of this.gameObjects){
			for(var mesh of obj.meshes){
				this.scene.add(mesh);	
			}
		}
		// Render loop
		this.renderer.setAnimationLoop(()=>{this.render();});
	}

	createBusses(){
		var offset = Math.random() * (5 - 0.1) + 0.1;
		console.log(offset)
		this.busses.push(new Bus(new THREE.Vector3(-5,4,1)));
		this.busses.push(new Bus(new THREE.Vector3(-8-offset,4,1)));
		for(var bus of this.busses){
			this.gameObjects.push(bus);
		}
	}
	createCars(){
		this.cars.push(new Car(new THREE.Vector3(11,2,1)));

		var offset = Math.random() * (2 - 0.1) + 0.1;
		this.cars.push(new Car(new THREE.Vector3(14+offset,2,1)));
		
		var offset = Math.random() * (2 - 0.1) + 0.1;
		this.cars.push(new Car(new THREE.Vector3(17+offset,2,1)));
		for(var car of this.cars){
			this.gameObjects.push(car);
		}
	}

	repositionTarget(){


		var pos = this.possibleTargetPositions[Math.floor(Math.random()*this.possibleTargetPositions.length)];
		this.target.position = new THREE.Vector3(pos,12,0);
		this.target.updatePosition();
	}

	createLogs(){
		this.logs.push(new Log(new THREE.Vector3(11,11,1)));

		var offset = Math.random() * (2 - 0.1) + 0.1;
		this.logs.push(new Log(new THREE.Vector3(14+offset,11,1)));
		
		var offset = Math.random() * (2 - 0.1) + 0.1;
		this.logs.push(new Log(new THREE.Vector3(17+offset,11,1)));


		this.logs.push(new Log(new THREE.Vector3(11,8,1)));

		var offset = Math.random() * (2 - 0.1) + 0.1;
		this.logs.push(new Log(new THREE.Vector3(14+offset,8,1)));
		
		var offset = Math.random() * (2 - 0.1) + 0.1;
		this.logs.push(new Log(new THREE.Vector3(17+offset,8,1)));

		for(var log of this.logs){
			this.gameObjects.push(log);
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
			log.position.x -= this.log_speed;
			if(log.position.x < -1){
				log.position.x = 11;
			}
			log.updatePosition();
		}
	}
}