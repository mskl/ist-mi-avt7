class Grass{
    meshes = []
    shapeSize = [10,1,1]
    constructor(position){
        this.position = position;
        this.initializeObject();
    }

    initializeObject(){

        var geometry = new THREE.BoxGeometry(this.shapeSize[0],this.shapeSize[1],this.shapeSize[2]);
        var material = new THREE.MeshLambertMaterial( { color: 0x00ff00 } );
        this.mesh = new THREE.Mesh( geometry, material );
        this.mesh.position.set(this.position.x+this.shapeSize[0]/2, this.position.y, this.position.z);
        this.mesh.receiveShadow = true;
        this.meshes.push(this.mesh)
    }
    render(){
        
    }
}