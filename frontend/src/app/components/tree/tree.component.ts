import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { FlatTreeControl } from "@angular/cdk/tree"
import { MatTreeFlatDataSource, MatTreeFlattener } from "@angular/material/tree"
import { faFolder } from '@fortawesome/free-solid-svg-icons'
import { faFolderOpen } from '@fortawesome/free-solid-svg-icons'
import { faFileLines } from '@fortawesome/free-solid-svg-icons'


interface BlockNode{
  name: string;
  path: string;
  type: string;
  size: string;
  content: string;
  sons: BlockNode[];
}

interface ExampleFlatNode{
  expandable:boolean;
  name:string;
  path: string;
  type: string;
  level:number;
}

@Component({
  selector: 'app-tree',
  templateUrl: './tree.component.html',
  styleUrls: ['./tree.component.css']
})
export class TreeComponent implements OnInit {

  faFolder = faFolder;
  faFolderOpen = faFolderOpen;
  faFileLines = faFileLines;

  private _transformer = (node:BlockNode, level: number) => {
    return {
      expandable: !!node.sons && node.sons.length > 0,
      name: node.name,
      path: node.path,
      type: node.type,
      level:level,
    };
  }
  treeControl = new FlatTreeControl<ExampleFlatNode>(
    node=>node.level,
    node=>node.expandable
  );

  constructor(private router: Router) {
    //Tenemos que setear el arbol
    let arbolito = JSON.parse(localStorage.getItem('tree') || '{}') as BlockNode;
    this.setIdToChild('', arbolito);
    this.dataSource.data = [arbolito];
  }

  ngOnInit(): void {
    //Verificando si ya esta logeado
    if(localStorage.getItem('logeado') == 'false'){this.router.navigate(['/login']);}
  }

  treeFlatterner = new MatTreeFlattener(
    this._transformer,
    node=>node.level,
    node=>node.expandable,
    node=>node.sons
  );

  dataSource = new MatTreeFlatDataSource(
    this.treeControl,
    this.treeFlatterner
  );

  hasChild = (_: number, node: ExampleFlatNode) => node.expandable;

  setIdToChild(path:string, node: BlockNode){
    //Cambiamos el nodo actual
    let newPath = path + '|' + node.name;
    if(node.type == '0'){
      node.path = '/directory/' + newPath;
    } else {
      node.path = '/file/' + newPath;
    }
    for(let i = 0; i < node.sons.length; i++){
      this.setIdToChild(newPath, node.sons[i]);
    }
  }
}
