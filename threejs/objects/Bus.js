class Bus{
    meshes = []
    colliders = []
    wheelRotation = 0.1;
    clippingPlanes = [];
    constructor(position,clippingPlanes){
        this.position = position;
        if(clippingPlanes != null){
            this.clippingPlanes = clippingPlanes;
        }
        this.initializeObject();
    }

    initializeObject(){
        // Body
        var bodyGeometry = new THREE.BoxGeometry(3, 0.8, 1 );
        // Geometry
        var material = new THREE.MeshPhongMaterial( {
            shininess: 100,
            color: "rgb(255, 0, 0)",
            side: THREE.DoubleSide,
            clippingPlanes: this.clippingPlanes,
            clipShadows: true
        } );
        this.meshBody = new THREE.Mesh( bodyGeometry, material );
        this.meshBody.receiveShadow = true;
        this.meshBody.castShadow = true;
        this.meshes.push(this.meshBody);
        // Geometry
        var materialWheels = new THREE.MeshPhongMaterial( {
            shininess: 100,
            color: "rgb(0, 0, 0)",
            side: THREE.DoubleSide,
            clippingPlanes: this.clippingPlanes,
            clipShadows: true
        } );
        // Front Left wheel
        var geometry = new THREE.BoxGeometry(0.3, 0.1, 0.3 );
        var meshWheel = new THREE.Mesh( geometry, materialWheels );
        meshWheel.receiveShadow = true;
        meshWheel.castShadow = true;
        this.meshes.push(meshWheel);


        // Front Right wheel
        var geometry = new THREE.BoxGeometry(0.3, 0.1, 0.3 );
        var meshWheel = new THREE.Mesh( geometry, materialWheels );
        meshWheel.receiveShadow = true;
        meshWheel.castShadow = true;
        this.meshes.push(meshWheel);

        // Back Left wheel
        var geometry = new THREE.BoxGeometry(0.3, 0.1, 0.3 );
        var meshWheel = new THREE.Mesh( geometry, materialWheels );
        meshWheel.receiveShadow = true;
        meshWheel.castShadow = true;
        this.meshes.push(meshWheel);


        // Back Right wheel
        var geometry = new THREE.BoxGeometry(0.3, 0.1, 0.3 );
        var meshWheel = new THREE.Mesh( geometry, materialWheels );
        meshWheel.receiveShadow = true;
        meshWheel.castShadow = true;
        this.meshes.push(meshWheel);

        this.updatePosition();

        // Colliders
        this.meshBody.geometry.computeBoundingBox();
        this.boundingBox = this.meshBody.geometry.boundingBox.clone();


        this.boxHelper = new THREE.BoxHelper( this.meshBody, 0xffff00 );
        this.colliders.push(this.boxHelper);        
    }

    updatePosition(){
        this.meshes[0].position.set(this.position.x, this.position.y, this.position.z+0.1);
        this.meshes[1].position.set(this.position.x-1, this.position.y-0.5, this.position.z-0.35);
        this.meshes[2].position.set(this.position.x-1, this.position.y+0.5, this.position.z-0.3);
        this.meshes[3].position.set(this.position.x+1, this.position.y-0.5, this.position.z-0.3);
        this.meshes[4].position.set(this.position.x+1, this.position.y+0.5, this.position.z-0.3);
    }

    render(){
        this.meshBody.updateMatrixWorld( true );
        this.boundingBox.copy( this.meshBody.geometry.boundingBox ).applyMatrix4( this.meshBody.matrixWorld );

        this.boxHelper.update();

        this.meshes[1].rotation.y += this.wheelRotation;
        this.meshes[2].rotation.y += this.wheelRotation;
        this.meshes[3].rotation.y += this.wheelRotation;
        this.meshes[4].rotation.y += this.wheelRotation;

    }
}