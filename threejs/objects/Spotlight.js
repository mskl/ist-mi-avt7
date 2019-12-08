class Spotlight{

	light_spot_color = 0xDDDDDD;
	light_spot_intensity = 0.7;
	light_spot_position = {x: 5, y: 6, z: 10}
	light_spot_camera_near = 0.5;
	light_spot_shadow_darkness = 0.35;

    constructor(position){
		this.spot_light = new THREE.SpotLight(this.light_spot_color, this.light_spot_intensity);
		this.spot_light.position.set(position.x, position.y, position.z);
		this.spot_light.target = this.scene;
		this.spot_light.castShadow = true;
		this.spot_light.receiveShadow = true;
		this.spot_light.shadowDarkness = this.light_spot_shadow_darkness;
		this.spot_light.shadowCameraNear	= this.light_spot_camera_near;		
		//this.spot_light.add( this.lensflare );
		this.spot_light.distance = 50;
		this.spot_light.penumbra = 0.25;
		this.spot_light.angle = 0.4;
		this.targetObject = new THREE.Object3D(); 
		this.targetObject.position.set(position.x, position.y,0);
        this.spot_light.target = this.targetObject;
    }


	toggleLight(state){
		if(state){
			this.spot_light.intensity = this.light_spot_intensity;
		}else{
			this.spot_light.intensity = 0;
		}
	}

}