class River{
    meshes = []
    shapeSize = [9.9,7,0.8]

    TAG = "RIVER"
    constructor(position){
        this.position = position;
        this.initializeObject();
    }

    initializeObject(){

        var geometry = new THREE.BoxGeometry(this.shapeSize[0],this.shapeSize[1],this.shapeSize[2]);
        var material = new THREE.MeshBasicMaterial( { color: 'skyblue', transparent: true, opacity: 0.5 } );
        this.mesh = new THREE.Mesh( geometry, material );
        this.mesh.position.set(this.position.x+this.shapeSize[0]/2+0.05, this.position.y, this.position.z);
        this.meshes.push(this.mesh)


        // Colliders
        this.mesh.geometry.computeBoundingBox();
        this.boundingBox = this.mesh.geometry.boundingBox.clone();

        this.mesh.updateMatrixWorld( true );
        this.boundingBox.copy( this.mesh.geometry.boundingBox ).applyMatrix4( this.mesh.matrixWorld );
        
        this.boxHelper = new THREE.BoxHelper( this.mesh, 0xffff00 );
        this.meshes.push(this.boxHelper);
    }
    render(){
        
    }
}