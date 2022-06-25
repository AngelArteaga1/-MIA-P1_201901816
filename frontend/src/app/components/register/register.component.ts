import { Component, OnInit } from '@angular/core';
import { Register } from 'src/app/models/Register/register';
import { Router, UrlSerializer } from '@angular/router';
import { HttpClient } from '@angular/common/http';
import { Observable, of } from 'rxjs';

@Component({
  selector: 'app-register',
  templateUrl: './register.component.html',
  styleUrls: ['./register.component.css']
})
export class RegisterComponent implements OnInit {

  private _jsonURL = 'assets/files/users.json';
  
  constructor(private router: Router, private http: HttpClient) { }

  registerModel = new Register('', '', '', '', '', '', '', new Date(0), new Date(), false, false);
  errorMessage = "";
  error = false;

  ngOnInit(): void {
  }

  public getJSON(): Observable<any> {
    return this.http.get(this._jsonURL);
  }

  public isNumber(char: string): boolean{
    if(char == "0") { return true; }
    if(char == "1") { return true; }
    if(char == "2") { return true; }
    if(char == "3") { return true; }
    if(char == "4") { return true; }
    if(char == "5") { return true; }
    if(char == "6") { return true; }
    if(char == "7") { return true; }
    if(char == "8") { return true; }
    if(char == "9") { return true; }
    return false;
  }

  public isSymbol(char: string): boolean{
    return !(/^[0-9a-zA-Z]+$/.test(char));
  }

  public hasUpperCase(password: string): boolean{
    for(let i = 0; i < password.length; i++){
      if(this.isNumber(password.charAt(i))){
        continue;
      }
      if(password.charAt(i).toUpperCase() === password.charAt(i)){
        return true;
      }
    }
    return false;
  }

  public hasNumber(password: string): boolean{
    for(let i = 0; i < password.length; i++){
      if(this.isNumber(password.charAt(i))){
        return true;
      }
    }
    return false;
  }

  public hasSymbol(password: string): boolean{
    for(let i = 0; i < password.length; i++){
      if(this.isSymbol(password.charAt(i))){
        return true;
      }
    }
    return false;
  }

  public validateEmail(email: string){
    return String(email)
      .toLowerCase()
      .match(
        /^(([^<>()[\]\\.,;:\s@"]+(\.[^<>()[\]\\.,;:\s@"]+)*)|(".+"))@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,}))$/
      );
  };

  public validatePassword(password: string): boolean{
    if(password.length < 8){
      this.errorMessage = "Error: El campo contraseña tiene que tener minimo 8 caracteres"
      this.error = true;
      return false;
    }
    if(!this.hasUpperCase(password)){
      this.errorMessage = "Error: El campo contraseña tiene que tener minimo un caracter en mayuscula"
      this.error = true;
      return false;
    }
    if(!this.hasNumber(password)){
      this.errorMessage = "Error: El campo contraseña tiene que tener minimo un caracter de numero"
      this.error = true;
      return false;
    }
    if(!this.hasSymbol(password)){
      this.errorMessage = "Error: El campo contraseña tiene que tener minimo un caracter de simbolo"
      this.error = true;
      return false;
    }
    return true;
  }

  public validate(): boolean{
    if(this.registerModel.nombre == ''){
      this.errorMessage = "Error: El campo nombre es obligatorio"
      this.error = true;
      return false;
    }
    if(this.registerModel.apellido == ''){
      this.errorMessage = "Error: El campo apellido es obligatorio"
      this.error = true;
      return false;
    }
    if(this.registerModel.username == ''){
      this.errorMessage = "Error: El campo username es obligatorio"
      this.error = true;
      return false;
    }
    if(this.registerModel.email == ''){
      this.errorMessage = "Error: El campo email es obligatorio"
      this.error = true;
      return false;
    }
    if(this.registerModel.password == ''){
      this.errorMessage = "Error: El campo contraseña es obligatorio"
      this.error = true;
      return false;
    }
    if(this.registerModel.confirmPassword == ''){
      this.errorMessage = "Error: El campo confimacion de contraseña es obligatorio"
      this.error = true;
      return false;
    }
    if(this.validateEmail(this.registerModel.email) == null){
      this.errorMessage = "Error: El campo email no tiene el formato correcto"
      this.error = true;
      return false;
    }
    if(!this.validatePassword(this.registerModel.password)){
      return false;
    }
    if(this.registerModel.password != this.registerModel.confirmPassword){
      this.errorMessage = "Error: El campo de confirmar contraseña no es el mismo que el contraseña"
      this.error = true;
      return false;
    }
    return true;
  }

  onSubmit(){
    //Tenemos que verificar si se encuentra el usuario
    this.getJSON().subscribe(data => {
      //Primero validamos los campos
      if(!this.validate()){
        return;
      }

      //Tenemos que iterar la lista
      for(let i = 0; i < data.length; i++){
        //Tenemos que verificar si existe el usuario
        let user = data[i];
        if(user.email == this.registerModel.email){
            //Ingresamos el usuario en el localstorage
            this.errorMessage = "Error: Ya existe ese usuario"
            this.error = true;
            return;
        }
      }
      //Agregamos el usuario
      data.push(this.registerModel);
      console.log(data);
    });
  }

}
