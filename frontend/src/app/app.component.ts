import { Component } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable, of } from 'rxjs';


@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
  title = 'frontend';

  constructor(private http: HttpClient){
    this.grabar_localstorage();
  }

  private _jsonURL = 'assets/files/users.json';

  public getJSON(): Observable<any> {
    return this.http.get(this._jsonURL);
  }

  
  grabar_localstorage(){
    if(localStorage.getItem('usuarios') === null){
      //Tenemos que leer los usuarios del json
      this.getJSON().subscribe(data => {
        //Almacenamos la lista
        localStorage.setItem('usuarios', JSON.stringify(data));
      });
    }
    if(localStorage.getItem('tree') === null){
      //Tenemos que leer el tree del json
    }
    if(localStorage.getItem('logeado')=== null){
      //Seteamos la variable de logeo
      localStorage.setItem('logeado', 'false');
    }
  }

}
