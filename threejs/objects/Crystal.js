class Crystal{
    meshes = []
    shapeSize = [2,0.7,0.1]
    rotationSpeed = 0.1;
    constructor(position){
        this.position = position;
        this.initializeObject();
    }

    initializeObject(){

        var geometry = new THREE.ConeGeometry( 0.25, 0.5, 6 );
        var material  = new THREE.MeshBasicMaterial( {color: 0xffff00} );
        this.mesh = new THREE.Mesh( geometry, material );
        this.mesh.rotation.y = 10;
        this.mesh.rotation.x = 3.14/2;
        this.mesh.receiveShadow = true;
        this.mesh.castShadow = true;
        this.meshes.push(this.mesh)

        this.mesh = new THREE.Mesh( geometry, material );
        this.mesh.rotation.y = 10;
        this.mesh.rotation.x = -3.14/2;
        this.mesh.receiveShadow = true;
        this.mesh.castShadow = true;
        this.meshes.push(this.mesh)
        this.updatePosition();
    }
    render(){
        
        this.meshes[0].rotation.y += this.rotationSpeed;
        this.meshes[1].rotation.y -= this.rotationSpeed;
    }

    updatePosition(){
        this.meshes[0].position.set(this.position.x+0.5, this.position.y, 1+0.5);
        this.meshes[1].position.set(this.position.x+0.5, this.position.y, 1+0);
    }
    
}