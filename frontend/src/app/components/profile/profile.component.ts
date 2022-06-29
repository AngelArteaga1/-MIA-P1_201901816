import { Component, OnInit } from '@angular/core';
import { Register } from 'src/app/models/Register/register';
import { Router, ActivatedRoute } from '@angular/router';

@Component({
  selector: 'app-profile',
  templateUrl: './profile.component.html',
  styleUrls: ['./profile.component.css']
})
export class ProfileComponent implements OnInit {

  sub:any;
  registerModel = new Register('', '', '', '', '', '', '', new Date(0), new Date(), new Date(0), false, false);
  errorMessage = "";
  error = false;
  username = '';

  constructor(private _Activatedroute:ActivatedRoute, private router: Router) { }

  ngOnInit(): void {
    //Verificando si ya esta logeado
    if(localStorage.getItem('logeado') == 'false'){this.router.navigate(['/login']); return;}
    //Ahora obtenemos el usuario
    this.sub=this._Activatedroute.paramMap.subscribe(params => { 
      this.username = params.get('username') || ''; 
      //Ahora tenemos que buscar el usuario
      //Otenemos el listado de usuarios
      let data = JSON.parse(localStorage.getItem("usuarios") || '{}');
      //Iteramos el listado
      for(let i = 0; i < data.length; i++){
        //Tenemos que verificar si existe el usuario
        let user = data[i];
        if(user.username == this.username){
          //Significa que encontramos el usuario
          this.registerModel.nombre = user.nombre;
          this.registerModel.apellido = user.apellido;
          this.registerModel.username = user.username;
          this.registerModel.numero = user.numero;
          this.registerModel.email = user.email;
          this.registerModel.nacimiento = user.nacimiento.split('T')[0];
          this.registerModel.password = user.password;
          this.registerModel.confirmPassword = user.confirmPassword;
        }
      }
    });
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
    /*if(!this.hasSymbol(password)){
      this.errorMessage = "Error: El campo contraseña tiene que tener minimo un caracter de simbolo"
      this.error = true;
      return false;
    }*/
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
    //Primero validamos los campos
    if(!this.validate()){ return; }

    //Otenemos el listado de usuarios
    let data = JSON.parse(localStorage.getItem("usuarios") || '{}');

    //Tenemos que iterar la lista
    for(let i = 0; i < data.length; i++){
      //Tenemos que verificar si existe el usuario
      let user = data[i];
      if(user.username == this.username){
        //Ahora actualizamos el user
        data[i].nombre = this.registerModel.nombre;
        data[i].apellido = this.registerModel.apellido;
        data[i].numero = this.registerModel.numero;
        data[i].nacimiento = this.registerModel.nacimiento;
        data[i].password = this.registerModel.password;
        data[i].passwordConfirm = this.registerModel.confirmPassword;
        //Ahora actualizamos el listado de usuarios
        localStorage.setItem('usuarios', JSON.stringify(data));
        this.router.navigate(['/profileConfirm']);
        return;
      }
    }
    //Error
    this.errorMessage = "Error: Parece ser que ya no existe el usuario"
    this.error = true;
  }

}
