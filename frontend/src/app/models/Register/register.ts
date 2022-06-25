export class Register {
    constructor(
        public nombre: string,
        public apellido: string,
        public username: string,
        public email: string,
        public numero: string,
        public password: string,
        public confirmPassword: string,
        public nacimiento: Date,
        public fecha: Date,
        public administrador: boolean,
        public status: boolean
    ) {}
}
