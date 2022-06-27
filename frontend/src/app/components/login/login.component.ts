import { Component, OnInit } from '@angular/core';
import { Login } from 'src/app/models/Login/login';
import { Router } from '@angular/router';
import '../../../assets/js/smtp.js';
import { Location, LocationStrategy, PathLocationStrategy } from '@angular/common';
declare let Email: any;

@Component({
  selector: 'app-login',
  templateUrl: './login.component.html',
  styleUrls: ['./login.component.css']
})
export class LoginComponent implements OnInit {

  constructor(private router: Router, private location: Location) { 
  }

  loginModel = new Login('', '');
  errorMessage = "";
  successMessage = "";
  error = false;
  success = false;

  ngOnInit(): void {
  }

  validate():boolean{
    if(this.loginModel.email == ""){
      this.errorMessage = "Error: El campo email es obligatorio"
      this.error = true;
      return false;
    }
    if(this.loginModel.password == ""){
      this.errorMessage = "Error: El campo contraseña es obligatorio"
      this.error = true;
      return false;
    }
    return true;
  }

  validateRecover():boolean{
    if(this.loginModel.email == ""){
      this.errorMessage = "Error: El campo email es obligatorio"
      this.error = true;
      return false;
    }
    return true;
  }

  onSubmit(){
    //Validamos
    if(!this.validate()){ return; }
    //Otenemos el listado de usuarios
    let data = JSON.parse(localStorage.getItem("usuarios") || '{}');
    //Tenemos que iterar la lista
    for(let i = 0; i < data.length; i++){
      //Tenemos que verificar si existe el usuario
      let user = data[i];
      if(user.email == this.loginModel.email && user.password == this.loginModel.password && user.status == true){
          //Ingresamos el usuario en el localstorage
          localStorage.setItem("logeado", "true");
          localStorage.setItem("user", JSON.stringify(user));
          this.router.navigate(['/']);
      }
    }
    this.errorMessage = "Error: Usuario no encontrado"
    this.error = true;
    console.log(this.loginModel);
}

  redirectToRegister(){
    this.router.navigate(['/register']);
  }

  recoverPassword(){
    //Validamos
    if(!this.validateRecover()){ return; }
    //Otenemos el listado de usuarios
    let data = JSON.parse(localStorage.getItem("usuarios") || '{}');
    //Tenemos que iterar la lista
    for(let i = 0; i < data.length; i++){
      //Tenemos que verificar si existe el usuario
      let user = data[i];
      if(user.email == this.loginModel.email && user.status == true){
          //Tenemos que ingresar el momento en donde se genero el email
          data[i].recover = new Date();
          localStorage.setItem("usuarios", JSON.stringify(data));
          //Obtenemos el url para la recuperacion de la contraseña
          let url = window.location.origin + '/recoverPassword/' + data[i].username;
          console.log(url);
          //Inicializamos el html del email
          let htmlEmail = `<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
          <html xmlns="http://www.w3.org/1999/xhtml" lang="EN">
            <head>
              <style type="text/css">
                @media screen {
                  @font-face {
                    font-family: "Lato";
                    font-style: normal;
                    font-weight: 400;
                    src: local("Lato Regular"), local("Lato-Regular"), url(https://fonts.gstatic.com/s/lato/v11/qIIYRU-oROkIk8vfvxw6QvesZW2xOQ-xsNqO47m55DA.woff) format("woff");
                  }
                  body, html {
                    margin: 0px;
                    padding: 0px;
                    -webkit-font-smoothing: antialiased;
                    text-size-adjust: none;
                    width: 100% !important;
                    background: #F9F9FF;
                    font-family: "Lato", "Lucida Grande", "Lucida Sans Unicode", Tahoma, Sans-Serif;
                    word-break: break-word;
                  }
                  .contentMainTable {
                    background: #FFFFFF;
                    border: 1px solid #EEEEFF;
                    margin-top: 98px;
                    margin-bottom: 69px;
                    margin-left: auto;
                    margin-right: auto;
                    width: 600px;
                    height: 503px;
                  }
                  .ExternalClass, .ExternalClass p, .ExternalClass span, .ExternalClass font, .ExternalClass td, .ExternalClass div {
                    line-height: 100%;
                  }
                  .ExternalClass {
                    width: 100%;
                  }
                  .logoImage {
                    margin-top: -50px;
                    padding-bottom: 7px;
                  }
                  h1 {
                    font-weight: bold;
                    font-size: 30px;
                    font-family: "Lato";
                    letter-spacing: 0px;
                    color: #25254E;
                  }
                  p {
                    font-weight: 300;
                    font-size: 14px;
                    letter-spacing: 0px;
                    color: #4D4D80;
                  }
                  .greyLine {
                    border: 1px solid #CED7F7;
                    width: 100%;
                    margin-top: 32px
                  }
                  h2 {
                    font-weight: bold;
                    font-size: 15px;
                    letter-spacing: 0px;
                    color: #25254E;
                  }
                  h3 {
                    font-weight: 300;
                    font-size: 15px;
                    letter-spacing: 0px;
                    color: #4D4D80;
                  }
                  .footer {
                    margin-top: 32px;
                    margin-bottom: 20px;
                    font-size: 11px;
                    font-weight: 300px;
                    color: #4D4D80;
                  }
                  .footerIcons img {
                    margin-left: 11px;
                    margin-right: 11px;
                  }
                  .blueButton {
                    background: #8AA1EB;
                    border-radius: 10px;
                    padding: 17px 35px;
                    border: none;
                    color: #FFFFFF;
                    font-size: 15px;
                    margin-bottom: 32px;
                    cursor: pointer;
                  }
                  .blueButton:focus {
                    outline: none;
                    border: 2px solid #5457FF;
                    padding: 15px 33px;
                  }
                  @media only screen and (max-width: 480px) {
                    table, table tr td, table td {
                      width: 100%;
                    }
                    .contentMainTable {
                      width: 100%;
                      border: none;
                    }
                    body, html {
                      background: #FFFFFF;
                    }
                    h1 {
                      font-size: 24px;
                    }
                  }
                }
              </style>
              <title>
              </title>
              <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
            </head>
            <body style="padding:0; margin: 0;">
              <table border="0" cellpadding="0" cellspacing="0" height="100%" width="100%" id="body_table">
                <tbody>
                  <tr>
                    <td align="center" valign="top"> 
                      <table border="0" cellpadding="20" cellspacing="0" width="100%" class="contentMainTable">
                        <tbody>
                          <tr>
                            <td align="center" valign="top"> 
                              <table border="0" cellpadding="20" cellspacing="0" width="100%" id="content">
                                <tbody>
                                  <tr>
                                    <td align="center" valign="top">
                                      <span class="isDesktop">
                                        <h1 style="margin-bottom: 32px">
                                          Recuperacion de contraseña de FuBox!
                                        </h1>
                                        <p style="margin-top:0px">Podras tener acceso a todas las nuevas opciones de almacenamiento en la nube.
                                        </p></span>
                                      <div class="greyLine">
                                      </div>
                                    </td>
                                  </tr>
                                  <tr>
                                    <td align="center" valign="top">
                                      <h2>Seguridad
                                      </h2>
                                      <p>FuBox contiene un sistema de seguridad muy inteligente que no tendras que preocuparte mas por tus archivos.
                                      </p>
                                        <a target="_blank" href="` + url + `"><button class="blueButton">Cambiar Contraseña</button></a>
                                        <div class="greyLine"></div>
                                        <div class="footer">
                                          angel.arteaga01.aa@gmail.com
                                        </div>
                                    </td>
                                </tbody>
                              </table>
                            </td>
                          </tr>
                        </tbody>
                      </table>
                    </td>
                  </tr>
                </tbody>
              </table>
            </body>
          </html>`
          //Ahora mandamos el email
          Email.send({
            Host : 'smtp.elasticemail.com',
            Username : 'angel.arteaga01.aa@gmail.com',
            Password : '60370CBD05FC5D40D1691ED8340452947BEF',
            To : data[i].email,
            From : 'angel.arteaga01.aa@gmail.com',
            Subject : 'Recuperacion de Contraseña - FuBox',
            Body : htmlEmail
          }).then( console.log("Pues todo bien bro jaja") );
          //Ingresamos el mensaje de confirmacion
          this.success = true;
          this.error = false;
          this.successMessage = "Correo enviado: Revise su correo para poder cambiar la contraseña"
          return;
      }
    }
    //Si no se encontro el usuario
    this.errorMessage = "Error: Usuario no encontrado"
    this.error = true;
    //console.log(this.loginModel);
  }

}
