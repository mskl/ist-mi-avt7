class Log{
    meshes = []
    shapeSize = [2,0.7,0.1]
    constructor(position){
        this.position = position;
        this.initializeObject();
    }

    initializeObject(){

        var geometry = new THREE.BoxGeometry(this.shapeSize[0],this.shapeSize[1],this.shapeSize[2]);
        var material = new THREE.MeshLambertMaterial( { color: "rgb(150, 75, 0)" } );
        this.mesh = new THREE.Mesh( geometry, material );
        this.mesh.receiveShadow = true;
        this.mesh.castShadow = true;
        this.meshes.push(this.mesh)
        this.updatePosition();
    }
    render(){
        
    }

    updatePosition(){
        this.meshes[0].position.set(this.position.x+this.shapeSize[0]/2, this.position.y, 0.4);
    }
}