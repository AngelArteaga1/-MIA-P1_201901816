import { Register } from 'src/app/models/Register/register';
import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import '../../../assets/js/smtp.js';
declare let Email: any;

@Component({
  selector: 'app-users',
  templateUrl: './users.component.html',
  styleUrls: ['./users.component.css']
})
export class UsersComponent implements OnInit {

  users = JSON.parse(localStorage.getItem("usuarios") || '{}') as Array<Register>;

  htmlEmail = `<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
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
            height: 603px;
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
                                  Bienvenido a FuBox!
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
                            </td>
                          </tr>
                          <tr>
                            <td align="center" valign="top">
                              <h2>Configuracion
                              </h2>
                              <p>Podras configurar los datos de tu cuenta nuevo una vez entres a nuestro sitio web.
                              </p>
                            </td>
                          </tr>
                          <tr>
                            <td align="center" valign="top">
                              <p>Gracias por escoger al mejor servicio de almacenamiento online.
                              </p>
                              <div class="greyLine">
                              </div>
                              <div class="footer">
                                angel.arteaga01.aa@gmail.com
                              </div>
                            </td>
                          </tr>
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

  constructor(private router: Router) { }

  ngOnInit(): void {
    //Verificando si ya esta logeado
    if(localStorage.getItem('logeado') == 'false'){this.router.navigate(['/login']);}
    //Verificamos si el usuario es administrador
    let user = JSON.parse(localStorage.getItem("user") || '{}');
    if(user.administrador == false){this.router.navigate(['/withoutAccess']);}
  }

  changeStatus(event: Event) {
    let chequesito = event.target as HTMLInputElement;
    let status = false;
    if (chequesito?.checked) {
      status = true;
      console.log('Checkbox is checked');
    } else {
      status = false;
      console.log('Checkbox is NOT checked');
    }
    console.log((event.target as HTMLInputElement).id);
    //Ahora tenemos que encontrar el id de 
    let data = JSON.parse(localStorage.getItem("usuarios") || '{}');
    for(let i = 0; i < data.length; i++){
      //Buscamos el usuario
      if(data[i].username == chequesito.id){
        //Actualizamos el usuarios
        data[i].status = status;
        localStorage.setItem("usuarios", JSON.stringify(data));
        //Ahora enviamos el email
        if(chequesito?.checked){
          Email.send({
            Host : 'smtp.elasticemail.com',
            Username : 'angel.arteaga01.aa@gmail.com',
            Password : '60370CBD05FC5D40D1691ED8340452947BEF',
            To : data[i].email,
            From : 'angel.arteaga01.aa@gmail.com',
            Subject : 'Confirmacion de Usuario - FuBox',
            Body : this.htmlEmail
          }).then( console.log("Pues todo bien bro jaja") );
        }
      }
    }
  }
}
