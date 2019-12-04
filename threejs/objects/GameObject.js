import * as THREE from 'three';
class GameObject{
    position = new THREE.Vector3(0,0,0);    
    constructor(position){
        this.position = position;
    }
}