class Car{
    meshes = []
    wheelRotation = -0.1;
    constructor(position){
        this.position = position;
        this.initializeObject();
    }

    initializeObject(){
        // Body
        var bodyGeometry = new THREE.BoxGeometry(1.5, 0.8, 0.6 );
        var material = new THREE.MeshBasicMaterial( { color: "rgb(255,255,0)"} );
        var meshBody = new THREE.Mesh( bodyGeometry, material );
        meshBody.receiveShadow = true;
        meshBody.castShadow = true;
        this.meshes.push(meshBody)


        var materialWheels = new THREE.MeshBasicMaterial( { color: "rgb(0,0,0)"} );
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
    }

    updatePosition(){
        this.meshes[0].position.set(this.position.x-0.05, this.position.y, this.position.z-0.1);
        this.meshes[1].position.set(this.position.x-0.5, this.position.y-0.5, this.position.z-0.3);
        this.meshes[2].position.set(this.position.x-0.5, this.position.y+0.45, this.position.z-0.3);
        this.meshes[3].position.set(this.position.x+0.5, this.position.y-0.5, this.position.z-0.3);
        this.meshes[4].position.set(this.position.x+0.5, this.position.y+0.45, this.position.z-0.3);
    }

    render(){
        this.meshes[1].rotation.y += this.wheelRotation;
        this.meshes[2].rotation.y += this.wheelRotation;
        this.meshes[3].rotation.y += this.wheelRotation;
        this.meshes[4].rotation.y += this.wheelRotation;

    }
}